#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

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

// General setup
const uint16_t  monitorBaud = 9600; 
const uint16_t  sampleTime  = 1000; // Loop delay [ms]
const float     s_to_ms     = 1000;

// BME280 setup
const uint8_t i2cAddress           = 0x76;
const float   seaLevelPressure_hPa = 1013.25f;
const float   Pa_to_hPa            = 1.0f/100.0f;

Adafruit_BME280 bme;

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

    pinMode(MISO,OUTPUT);   //Sets MISO as OUTPUT
    SPCR |= _BV(SPE);       //Turn on SPI in Slave Mode
    SPI.attachInterrupt();  //Activate SPI Interupt

    Serial.begin(monitorBaud);

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