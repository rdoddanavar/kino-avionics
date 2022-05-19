#include <Arduino.h>
#include <SPI.h>

//volatile byte receivedData;

ISR (SPI_STC_vect)        //Inerrrput routine function 
{
    SPDR = 5;
}

void setup()
{
    Serial.begin(9600);

    pinMode(MISO,OUTPUT);   //Sets MISO as OUTPUT
    SPCR |= _BV(SPE);       //Turn on SPI in Slave Mode
    SPI.attachInterrupt();  //Activate SPI Interuupt 
}

void loop()
{ 

    Serial.println(5, HEX);
    delay(500);

}