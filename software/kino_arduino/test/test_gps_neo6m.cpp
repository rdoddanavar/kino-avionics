#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

//----------------------------------------------------------------------------//

// General setup
const uint8_t rxPin = 9;
const uint8_t txPin = 8;

const uint16_t monitorBaud = 9600; 
const uint16_t gpsBaud     = 9600;
const uint16_t sampleTime  = 1000; // Loop delay [ms]

String dataOut;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(rxPin, txPin);

void smart_delay(unsigned long ms);

//----------------------------------------------------------------------------//

void setup()
{

    Serial.begin(monitorBaud);
    ss.begin(gpsBaud);
    dataOut = "";

}

void loop()
{

    dataOut += "Sat: "  + String(gps.satellites.value())          + ", ";
    dataOut += "HDOP: " + String(gps.hdop.hdop())                 + ", ";
    dataOut += "Lat: "  + String(gps.location.lat())     + " deg" + ", ";
    dataOut += "Lng: "  + String(gps.location.lng())     + " deg" + ", ";
    dataOut += "Age: "  + String(gps.location.age())     + " ms"  + ", ";
    dataOut += "Alt: "  + String(gps.altitude.meters())  + " m"   + ", ";
    dataOut += "Spd: "  + String(gps.speed.mps())        + " mps"       ;

    Serial.println(dataOut);
    dataOut = "";

    smart_delay(sampleTime);

    if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

}

// This custom version of delay() ensures that the gps object is being "fed"
void smart_delay(unsigned long ms)
{
    
    unsigned long start = millis();

    do
    {
        while (ss.available())
        {
            gps.encode(ss.read());
        }
    } while (millis() - start < ms);

}