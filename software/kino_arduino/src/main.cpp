#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//----------------------------------------------------------------------------//

// SPI setup

#define SAMPLE_TIME (10.0) // Loop delay [ms] 

const unsigned int nByte = 4;
volatile byte iByte = 0x00;

typedef union
{
    float value;
    uint8_t bytes[nByte];
} FloatUnion;

volatile FloatUnion *dataOut = 0;

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

    //Serial.println(char(SPDR));

    switch (char(SPDR))
    {
        case 't':
            iByte   = 0x00;
            dataOut = &dataTime;
            break;
        case 'm':
            iByte   = 0x00;
            dataOut = &dataTemp;
            break;
        case 'p':
            iByte   = 0x00;
            dataOut = &dataPress;
            break;
        case 'a':
            iByte   = 0x00;
            dataOut = &dataAlt;
            break;
        case 'h':
            iByte   = 0x00;
            dataOut = &dataHum;
            break;
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