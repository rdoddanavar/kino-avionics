#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

const int rxPin = 9;
const int txPin = 8;

const int monitorBaud = 9600; 
const int gpsBaud     = 9600;

// The TinyGPSPlus object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(rxPin, txPin);

void smart_delay(unsigned long ms);

String dataOut;

void setup()
{
    
    Serial.begin(monitorBaud);
    ss.begin(gpsBaud);
    dataOut = "";

}

void loop()
{

    dataOut = "";

    dataOut += "Sat: "  + String(gps.satellites.value())          + ", ";
    dataOut += "HDOP: " + String(gps.hdop.hdop())                 + ", ";
    dataOut += "Lat: "  + String(gps.location.lat())     + " deg" + ", ";
    dataOut += "Lng: "  + String(gps.location.lng())     + " deg" + ", ";
    dataOut += "Age: "  + String(gps.location.age())     + " ms"  + ", ";
    dataOut += "Alt: "  + String(gps.altitude.meters())  + " m"   + ", ";
    dataOut += "Spd: "  + String(gps.speed.mps())        + " mps"       ;

    Serial.println(dataOut);

    smart_delay(1000);

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