#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//----------------------------------------------------------------------------//

// SPI setup

#define SAMPLE_TIME (100.0) // Loop delay [ms] 

const unsigned int nByte = 4;
volatile byte iByte = 0x00;

typedef union
{
    float value;
    uint8_t bytes[nByte];
} FloatUnion;

FloatUnion *dataOut;

FloatUnion dataTime;  // [s]
FloatUnion dataTemp;  // [deg C]
FloatUnion dataPress; // [hPa]
FloatUnion dataAlt;   // [m]
FloatUnion dataHum;   // [%]

//----------------------------------------------------------------------------//

// BME280 setup

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

//----------------------------------------------------------------------------//

// Interrupt routine function

ISR (SPI_STC_vect)
{
    
    switch (SPDR)
    {
        case 't':
            iByte   = 0x00;
            dataOut = &dataTime;
        default:
            SPDR = dataOut->bytes[iByte++];
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