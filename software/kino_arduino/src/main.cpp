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

//----------------------------------------------------------------------------//

// General setup
const float sampleRate = 1.0f;   // [Hz]
const float s_to_ms    = 1.0e3f;

uint16_t nDelay; // [ms]

//----------------------------------------------------------------------------//

// SPI setup
const    uint8_t keyStart = 48;
volatile bool    keyValid = 0;

const    uint16_t nByte = 4, nData = 5;
volatile uint16_t iByte = 0, iData = 0;

typedef union
{
    float value;
    uint8_t bytes[nByte];
} FloatUnion;

FloatUnion dataTime;  // [s]
FloatUnion dataTemp;  // [deg C]
FloatUnion dataPress; // [hPa]
FloatUnion dataAlt;   // [m]
FloatUnion dataHum;   // [%]

FloatUnion *dataOut[nData] = {&dataTime, &dataTemp, &dataPress, &dataAlt, &dataHum};

//----------------------------------------------------------------------------//

// BME280 setup
const uint8_t i2cAddress           = 0x76;
const float   seaLevelPressure_hPa = 1013.25f;
const float   Pa_to_psi            = 0.0001450380f;
const float   m_to_ft              = 1.0f/0.3048f;

Adafruit_BME280 bme;

// NEO6M setup
const uint16_t gpsBaud = 9600;
const float    m_to_ft = 1.0f/0.3048f;

const uint8_t rxPin = 9;
const uint8_t txPin = 8;
SoftwareSerial ss(rxPin, txPin);

TinyGPSPlus gps;

//----------------------------------------------------------------------------//

// Interrupt routine function

ISR (SPI_STC_vect)
{

    int key = int(SPDR);

    if (key >= keyStart)
    {
        
        iData = key - keyStart; // Offset from char '0'

        if (iData < nData)
        {
            iByte    = 0; // Reset counter
            keyValid = true;
        }
        else
        {
            keyValid = false;
        }

    }
    else
    {
        keyValid = false;
    }
    
    if (keyValid && (iByte < nByte))
    {
        SPDR = dataOut[iData]->bytes[iByte++];
    }

}

//----------------------------------------------------------------------------//

void setup()
{

    nDelay  = (uint16_t) (1.0f/sampleRate)*s_to_ms;
    
    // Initialize SPI
    pinMode(MISO,OUTPUT);   //Sets MISO as OUTPUT
    SPCR |= _BV(SPE);       //Turn on SPI in Slave Mode
    SPI.attachInterrupt();  //Activate SPI Interupt

    // Initialize sensors

    if (!bme.begin(i2cAddress))
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

}

//----------------------------------------------------------------------------//

void loop()
{
    
    dataTime.value  = millis() / s_to_ms;
    dataTemp.value  = bme.readTemperature();
    dataPress.value = bme.readPressure() * Pa_to_hPa;
    dataAlt.value   = bme.readAltitude(seaLevelPressure_hPa);
    dataHum.value   = bme.readHumidity();

    delay(sampleTime);

}