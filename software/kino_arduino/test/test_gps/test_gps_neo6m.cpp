#include <Arduino.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

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

// NEO6M setup
const uint16_t gpsBaud = 9600;
const float    m_to_ft = 1.0f/0.3048f;

const uint8_t rxPin = 9;
const uint8_t txPin = 8;
SoftwareSerial ss(rxPin, txPin);

TinyGPSPlus gps;

// This custom version of delay() ensures that the gps object is being "fed"
void smart_delay(unsigned long ms);

//----------------------------------------------------------------------------//

void setup()
{

    UNITY_BEGIN();
    
    iSample = 0;
    nSample = (uint16_t) runTime*sampleRate;
    nDelay  = (uint16_t) (1.0f/sampleRate)*s_to_ms;

    ss.begin(gpsBaud);

}

//----------------------------------------------------------------------------//

void loop()
{

    if (iSample < nSample)
    {   
    
        dataOut = "";
        
        dataOut += "Sat: "  + String(gps.satellites.value())        + ", ";
        dataOut += "HDOP: " + String(gps.hdop.hdop())               + ", ";
        dataOut += "Lat: "  + String(gps.location.lat())            + " deg" + ", ";
        dataOut += "Lng: "  + String(gps.location.lng())            + " deg" + ", ";
        dataOut += "Age: "  + String(gps.location.age())            + " ms"  + ", ";
        dataOut += "Alt: "  + String(gps.altitude.meters()*m_to_ft) + " ft"  + ", ";
        dataOut += "Spd: "  + String(gps.speed.mps()*m_to_ft)       + " fps"       ;

        uint16_t len = dataOut.length() + 1;
        char     buf[len];

        dataOut.toCharArray(buf, len);
        TEST_MESSAGE(buf);

        smart_delay(nDelay);
        iSample++;

        if (millis() > 5000 && gps.charsProcessed() < 10)
        {
            TEST_MESSAGE("No GPS data received: check wiring");
        }

    }
    else
    {
        UNITY_END();
    }

}

//----------------------------------------------------------------------------//

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