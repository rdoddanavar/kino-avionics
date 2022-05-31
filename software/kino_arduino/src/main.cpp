#include <Arduino.h>
#include <SPI.h>

char message[] = "Hello World!";
byte length = 12;
volatile byte idx = 0; 

ISR (SPI_STC_vect)        //Inerrrput routine function 
{
    
    if (idx == length)
    {
        idx = 0;
    }

    SPDR = message[idx++];

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

    //Serial.println("Tick");
    //delay(500);

}