// General
#include <Arduino.h>
#include <SPI.h>

// BME280
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// NEO6M
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// MPU9250
#include <MPU9250.h>

//----------------------------------------------------------------------------//

// General setup
const float sampleRate = 1.0f;   // [Hz]
const float s_to_ms    = 1.0e3f;

uint16_t nDelay; // [ms]

//----------------------------------------------------------------------------//

// SPI setup
const    uint8_t keyStart = 48;
volatile bool    spiWrite = false;

const    uint8_t nByte = 4, nData = 18;
volatile uint8_t iByte = 0, iData =  0;

typedef union
{
    float value;
    uint8_t bytes[nByte];
} FloatUnion;

// General data
FloatUnion dataTime; // [s]

// BME280 data
FloatUnion dataTemp;     // [deg F]
FloatUnion dataPress;    // [psi]
FloatUnion dataAltPress; // [ft]
FloatUnion dataHum;      // [%]

// NEO6M data
FloatUnion dataLat;    // [deg]
FloatUnion dataLng;    // [deg]
FloatUnion dataAltGps; // [ft]
FloatUnion dataSpd;    // [ft/s]

// MPU9250 data
FloatUnion dataLinAccX; // [G]
FloatUnion dataLinAccY;
FloatUnion dataLinAccZ;

FloatUnion dataGyroX; // [deg/s]
FloatUnion dataGyroY;
FloatUnion dataGyroZ;

FloatUnion dataMagX; // [mG]
FloatUnion dataMagY;
FloatUnion dataMagZ;

FloatUnion *dataOut[nData] = {&dataTime   , &dataTemp   , &dataPress  , &dataAltPress, &dataHum,
                              &dataLat    , &dataLng    , &dataAltGps , &dataSpd     ,
                              &dataLinAccX, &dataLinAccY, &dataLinAccZ,
                              &dataGyroX  , &dataGyroY  , &dataGyroZ  ,
                              &dataMagX   , &dataMagY   , &dataMagZ   };

//----------------------------------------------------------------------------//

// BME280 setup
const uint8_t bmeI2C               = 0x76;
const float   seaLevelPressure_hPa = 1013.25f;
const float   Pa_to_psi            = 0.0001450380f;
const float   m_to_ft              = 1.0f/0.3048f;

Adafruit_BME280 bme;

// NEO6M setup
const uint16_t gpsBaud = 9600;

const uint8_t rxPin = 9;
const uint8_t txPin = 8;
SoftwareSerial ss(rxPin, txPin);

TinyGPSPlus gps;

// MPU9250 setup
const uint8_t mpuI2C  = 0x68;
const float   magDecl = 9.43f; // [deg]

MPU9250 mpu;

//----------------------------------------------------------------------------//

// Interrupt routine function

ISR (SPI_STC_vect)
{

    uint8_t key = SPDR;

    if (!spiWrite && (key >= keyStart))
    {
        
        iData = key - keyStart; // Offset from char '0'

        if (iData < nData)
        {
            spiWrite = true;
            iByte    = 0; // Reset counter
        }

    }
    
    if (spiWrite)
    {
        if (iByte < nByte)
        {
            SPDR = dataOut[iData]->bytes[iByte++];
        }
        else
        {
            spiWrite = false;
        }
    }

}

//----------------------------------------------------------------------------//

float degC_to_degF(float degC)
{
    return (degC*(9.0f/5.0f) + 32.0f);
}

//----------------------------------------------------------------------------//

void init_atmos()
{
    
    if (!bme.begin(bmeI2C))
    {
        Serial.println("Error: BME280 initialization");
        while (1);
    }

    Serial.println("Initialization Complete: Atmos");

}

//----------------------------------------------------------------------------//

void read_atmos()
{
    
    // Read BME280 data
    dataTemp.value     = degC_to_degF(bme.readTemperature());
    dataPress.value    = bme.readPressure()*Pa_to_psi;
    dataAltPress.value = bme.readAltitude(seaLevelPressure_hPa)*m_to_ft;
    dataHum.value      = bme.readHumidity();

}

//----------------------------------------------------------------------------//

void init_imu()
{
    
    if (!mpu.setup(mpuI2C))
    {
        Serial.println("Error: MPU9250 initialization");
        while (1);
    }

    mpu.setMagneticDeclination(magDecl);
    mpu.calibrateAccelGyro();
    mpu.calibrateMag();

    Serial.println("Initialization Complete: IMU");

}

//----------------------------------------------------------------------------//

void read_imu()
{

    if (mpu.available())
    {

        mpu.update_accel_gyro();
        mpu.update_mag();

        // Read MPU9250 data
        dataLinAccX.value = mpu.getLinearAccX();
        dataLinAccY.value = mpu.getLinearAccY();
        dataLinAccZ.value = mpu.getLinearAccZ();

        dataGyroX.value = mpu.getGyroX();
        dataGyroY.value = mpu.getGyroY();
        dataGyroZ.value = mpu.getGyroZ();

        dataMagX.value = mpu.getMagX();
        dataMagY.value = mpu.getMagY();
        dataMagZ.value = mpu.getMagZ();

    }

}

//----------------------------------------------------------------------------//

void init_gps()
{
    ss.begin(gpsBaud);
    Serial.println("Initialization Complete: GPS");
}

//----------------------------------------------------------------------------//

void read_gps()
{
    
    // Read NEO6M data

    if (ss.available())
    {
        gps.encode(ss.read());
    }

    dataLat.value    = gps.location.lat();
    dataLng.value    = gps.location.lng();
    dataAltGps.value = gps.altitude.feet();
    dataSpd.value    = gps.speed.mps()*m_to_ft;

}

//----------------------------------------------------------------------------//

void setup()
{

    nDelay = (uint16_t) (1.0f/sampleRate)*s_to_ms;
    //Serial.begin(9600); // Debug messages
    
    // Initialize SPI
    pinMode(MISO,OUTPUT);  // Sets MISO as OUTPUT
    SPCR |= _BV(SPE);      // Turn on SPI in Slave Mode
    SPI.attachInterrupt(); // Activate SPI Interupt

    // Initialize sensors
    init_atmos();
    init_imu();
    init_gps();

}

//----------------------------------------------------------------------------//
/*
void debug()
{
    
    // Read MPU9250 data
    Serial.print(dataLinAccX.value); Serial.print(", ");
    Serial.print(dataLinAccY.value); Serial.print(", ");
    Serial.print(dataLinAccZ.value); Serial.print(", ");

    Serial.print(dataGyroX.value); Serial.print(", ");
    Serial.print(dataGyroY.value); Serial.print(", ");
    Serial.print(dataGyroZ.value); Serial.print(", ");

    Serial.print(dataMagX.value); Serial.print(", ");
    Serial.print(dataMagY.value); Serial.print(", ");
    Serial.print(dataMagZ.value);

    Serial.println();

}
*/
//----------------------------------------------------------------------------//

void loop()
{
    
    dataTime.value = millis()/s_to_ms;

    // Read sensors
    read_atmos();
    read_imu();
    read_gps();

    //debug();

    delay(nDelay);

}