#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <unity.h> // Unit testing only

//----------------------------------------------------------------------------//

// General setup
const float runTime    = 60.0f;  // [s]
const float sampleRate = 1.0f;   // [Hz]
const float s_to_ms    = 1.0e3f;

uint16_t iSample;
uint16_t nSample;
uint16_t nDelay; // [ms]

String dataOut;

//----------------------------------------------------------------------------//

// BME280 setup
const uint8_t i2cAddress           = 0x76;
const float   seaLevelPressure_hPa = 1013.25f;
const float   Pa_to_psi            = 0.0001450380f;
const float   m_to_ft              = 1.0f/0.3048f;

Adafruit_BME280 bme;

//----------------------------------------------------------------------------//

float degC_to_degF(float degC)
{
    return (degC*(9.0f/5.0f) + 32.0f);
}

//----------------------------------------------------------------------------//

void setup()
{

    iSample = 0;
    nSample = (uint16_t) runTime*sampleRate;
    nDelay  = (uint16_t) (1.0f/sampleRate)*s_to_ms;
    
    UNITY_BEGIN();

    if (!bme.begin(i2cAddress))
    {
        TEST_MESSAGE("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

}

//----------------------------------------------------------------------------//

void loop()
{

    if (iSample < nSample)
    {   
        
        dataOut = "";

        dataOut += "Temp: "  + String(degC_to_degF(bme.readTemperature()))            + " F"   + ", ";
        dataOut += "Press: " + String(bme.readPressure()*Pa_to_psi)                   + " psi" + ", ";
        dataOut += "Alt: "   + String(bme.readAltitude(seaLevelPressure_hPa)*m_to_ft) + " ft"  + ", ";
        dataOut += "Hum: "   + String(bme.readHumidity())                             + " %"   + ""  ;

        uint16_t len = dataOut.length() + 1;
        char     buf[len];

        dataOut.toCharArray(buf, len);
        TEST_MESSAGE(buf);

        delay(nDelay);
        iSample++;

    }
    else
    {
        UNITY_END();
    }

}