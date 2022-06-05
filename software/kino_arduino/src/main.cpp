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

ISR (SPI_STC_vect) // Interrput routine function 
{
    
    //time.value = millis() / 1.0e3;
    
    if (SPDR == '!')
    {
        iByte = 0x00;
    }
    
    SPDR = dataTime.bytes[iByte++];

}

void setup()
{

    pinMode(MISO,OUTPUT);   //Sets MISO as OUTPUT
    SPCR |= _BV(SPE);       //Turn on SPI in Slave Mode
    SPI.attachInterrupt();  //Activate SPI Interupt

    dataTime.value = 123.45;

}

void loop()
{ 

}