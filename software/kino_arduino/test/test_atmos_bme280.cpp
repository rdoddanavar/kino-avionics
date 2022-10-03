#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//----------------------------------------------------------------------------//

// General setup
const int monitorBaud = 9600; 
const int sampleTime  = 1000; // Loop delay [ms]

String dataOut;

// BME280 setup
#define I2C_ADDRESS (0x76)
const float seaLevelPressure_hPa = 1013.25f;
Adafruit_BME280 bme;

//----------------------------------------------------------------------------//

void setup()
{

    Serial.begin(monitorBaud);

    if (!bme.begin(I2C_ADDRESS))
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
    dataOut += "Press: " + String(bme.readPressure() / 100.0f)            + "hPa" + ", ";
    dataOut += "Alt: "   + String(bme.readAltitude(seaLevelPressure_hPa)) + "deg" + ", ";
    dataOut += "Hum: "   + String(bme.readHumidity())                     + "%"   + ", ";

    Serial.println(dataOut);
    dataOut = "";

    delay(sampleTime);

}