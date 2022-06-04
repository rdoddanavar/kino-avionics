#include <Arduino.h>
#include <SPI.h>

const unsigned int nByte = 4;
unsigned int iByte;

typedef union
{
    float value;
    uint8_t bytes[nByte];
} FloatUnion;

FloatUnion time;

ISR (SPI_STC_vect) // Interrput routine function 
{
    
    time.value = millis() / 1.0e3;
    
    for (iByte = 0; iByte < nByte; iByte++)
    {

        SPDR = time.bytes[iByte];
    }

}

void setup()
{

    pinMode(MISO,OUTPUT);   //Sets MISO as OUTPUT
    SPCR |= _BV(SPE);       //Turn on SPI in Slave Mode
    SPI.attachInterrupt();  //Activate SPI Interupt

}

void loop()
{ 

}