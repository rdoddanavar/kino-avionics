#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//----------------------------------------------------------------------------//

// SPI setup
volatile bool keyValid = 0;

const    unsigned int nByte = 4, nData = 5;
volatile unsigned int iByte = 0, iData = 0;

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

// Sensor setup
#define SAMPLE_TIME (10.0) // Loop delay [ms] 

// BME280 setup
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;

//----------------------------------------------------------------------------//

// Interrupt routine function

ISR (SPI_STC_vect)
{

    int key = int(SPDR);

    if (key >= 48)
    {
        
        iData = key - 48; // Offset from char '0'

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

    dataTime.value = 0.0;

    //----------------------------//

    Serial.begin(9600);

	if (!bme.begin(0x76))
    {
		Serial.println("Could not find a valid BME280 sensor, check wiring!");
		while (1);
	}

    //////////////////////////////////

    Serial.print(millis() / 1.0e3); Serial.print(" , ");
	Serial.print(bme.readTemperature()); Serial.print(" , ");
	Serial.print(bme.readPressure() / 100.0f); Serial.print(" , ");
	Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA)); Serial.print(" , ");
	Serial.print(bme.readHumidity()); Serial.print(" , ");

    //////////////////////////////////

}

//----------------------------------------------------------------------------//

void loop()
{ 
    
    dataTime.value  = millis() / 1.0e3;
	dataTemp.value  = bme.readTemperature();
	dataPress.value = bme.readPressure() / 100.0f;
	dataAlt.value   = bme.readAltitude(SEALEVELPRESSURE_HPA);
	dataHum.value   = bme.readHumidity();

    delay(SAMPLE_TIME);

}