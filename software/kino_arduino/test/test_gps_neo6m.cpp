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

void setup()
{
    
    Serial.begin(115200);
    ss.begin(gpsBaud);

}

void loop()
{

    /*
    printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
    printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
    printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
    printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
    printInt(gps.location.age(), gps.location.isValid(), 5);
    printDateTime(gps.date, gps.time);
    printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
    printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
    printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
    */

    Serial.print("Satellites: "); Serial.print(gps.satellites.value());

    Serial.println();

    smart_delay(1000);

    if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

}

// This custom version of delay() ensures that the gps object
// is being "fed".
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