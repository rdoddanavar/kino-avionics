#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//----------------------------------------------------------------------------//

// General setup
const uint16_t monitorBaud = 9600; 
const uint16_t sampleTime  = 1000; // Loop delay [ms]

String dataOut;

// BME280 setup
const uint8_t i2cAddress           = 0x76;
const float   seaLevelPressure_hPa = 1013.25f;
const float   Pa_to_hPa            = 1.0f/100.0f;

Adafruit_BME280 bme;

//----------------------------------------------------------------------------//

void setup()
{

    Serial.begin(monitorBaud);

    if (!bme.begin(i2cAddress))
    {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

    dataOut = "";

}

//----------------------------------------------------------------------------//

void loop()
{

    dataOut += "Temp: "  + String(bme.readTemperature())                  + "*C"  + ", ";
    dataOut += "Press: " + String(bme.readPressure() * Pa_to_hPa)         + "hPa" + ", ";
    dataOut += "Alt: "   + String(bme.readAltitude(seaLevelPressure_hPa)) + "deg" + ", ";
    dataOut += "Hum: "   + String(bme.readHumidity())                     + "%"   + ""  ;

    Serial.println(dataOut);
    dataOut = "";

    delay(sampleTime);

}