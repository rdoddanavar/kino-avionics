#include <Arduino.h>
#include "SeeeduinoXIAO_SPISlave.h"

int dataOut = 0;

void setup()
{

    // Set all pins are input so we don't disrupt comminication on boot
    pinMode(SPI_MOSI_B_PIN, INPUT);
    pinMode(SPI_SCK_B_PIN, INPUT);
    pinMode(SPI_MISO_B_PIN, INPUT);
    pinMode(SPI_CS_B_PIN, INPUT);

    // init Serial USB
    Serial.begin(115200);//for debugging  

    //wait for serial to init
    while (!Serial); 

    // Seeduino XIAO - Enable SPI as Slave
    Seeeduino_spiSlave_init();

    #ifdef ENABLE_PORT_CRITICAL_SECTIONS
        Serial.println("ENABLE_PORT_CRITICAL_SECTIONS is defined from the start")
    #endif

}

void loop()
{ 
    dataOut += 1;
    Serial.println(dataOut);
    delay(500);
}

// Seeduino SPI interrupt routine
void SERCOM0_Handler()
{
    
    noInterrupts();
    uint8_t data = 0;
    uint8_t interrupts = SERCOM0->SPI.INTFLAG.reg; // Read SPI interrupt register

    if (interrupts & (1 << 3))
    {
        SERCOM0->SPI.INTFLAG.bit.SSL = 1; // Clear slave select interrupt
    }

    // This is where data is received, and is written to a buffer, which is used in the main loop
    if (interrupts & (1 << 2))
    {
        
        data = SERCOM0->SPI.DATA.reg;     // Read data register
        SERCOM0->SPI.INTFLAG.bit.RXC = 1; // Clear receive complete interrupt

        // Put the data somewhere, like a buffer - or print it for testing
        Serial.println(data, DEC); // Print received data as a number

    }

    // This is where data is transmitted
    if (interrupts & (1 << 1))
    {
        
        // TEST TEST TEST
        SERCOM0->SPI.DATA.reg = dataOut;
        // TEST TEST TEST
        
        SERCOM0->SPI.INTFLAG.bit.TXC = 1; //clear transmit complete interrupt
    }

    // Data Register Empty Interrupt
    if (interrupts & (1 << 0))
    {
        SERCOM0->SPI.DATA.reg = 0xAA;
    }
    
    interrupts();

}