#include <Arduino.h>
#include <SPI.h>

const unsigned int nByte = 4;
volatile byte iByte = 0x00;

typedef union
{
    float value;
    uint8_t bytes[nByte];
} FloatUnion;

FloatUnion dataTime;
FloatUnion *dataOut;

ISR (SPI_STC_vect) // Interrput routine function 
{
    
    switch (SPDR)
    {
        case 't':
            iByte = 0x00;
            //dataTime.value = millis() / 1.0e3;
            dataOut = &dataTime;
        default:
            SPDR = dataOut->bytes[iByte++];
    }

    //SPDR = dataOut->bytes[iByte++];

}

void setup()
{

    pinMode(MISO,OUTPUT);   //Sets MISO as OUTPUT
    SPCR |= _BV(SPE);       //Turn on SPI in Slave Mode
    SPI.attachInterrupt();  //Activate SPI Interupt

    dataTime.value = 0.0;

}

void loop()
{ 
    dataTime.value = millis() / 1.0e3;
    delay(10);
}