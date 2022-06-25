/*-----------------------------------------------------------------//
High Power Rocketry - Avionics Hardware & Software
MIT License
Copyright (c) 2020 Roshan Doddanavar
https://rdoddanavar.github.io
 _   _                       _          _       
| |_(_)_ _  ___ ___ __ ___ _(_)___ _ _ (_)__ ___
| / / | ' \/ _ \___/ _` \ V / / _ \ ' \| / _(_-<
|_\_\_|_||_\___/   \__,_|\_/|_\___/_||_|_\__/__/

Path:
    kino-avionics/software/kino_rpi/main.c
Created:
    2020-07-26
Type:
    C source code
Description:
    Kino Raspberry Pi flight software - exec routine
Dependencies:
    pigpio library
//-----------------------------------------------------------------*/

// Builtin libraries
#include <stdio.h>

// Project libraries
#include "pigpio.h"

// Types
#define nByte 4

typedef union
{
    float value;
    uint8_t bytes[nByte];
} FloatUnion;

FloatUnion dataTime; // [s]

int main(void)
{
    if (gpioInitialise() < 0)
    {
        printf("pigpio initialization failed\n");
    }
    else
    {
        
        printf("pigpio initialization successful\n");

        unsigned int spiChan  = 0;
        unsigned int baud     = 115200; // Min: 32000, Max: 125000000
        unsigned int spiFlags = 0;

        int handle = spiOpen(spiChan, baud, spiFlags);

        char buf[nByte];
        unsigned int count = 1;//nByte; // Number of bytes to read

        int status = 0;

        if (handle >= 0)
        {
            
            printf("SPI open successful!\n");

            float data[50];

            for (int iPing=0; iPing<50; iPing++)
            {
                
                buf[0] = 't';
                spiWrite(handle, buf, count);

                for (int iByte=0; iByte<nByte; iByte++)
                {
                    status = spiRead(handle, buf, count);
                    dataTime.bytes[iByte] = buf[0];
                }

                data[iPing] = dataTime.value;
                time_sleep(0.01);

            }

            spiClose(handle);

            for (int iPing=0; iPing<50; iPing++)
            {
                
                printf("Time (%d/%d): %.3f\n", iPing, 50, data[iPing]);

            }

        }
        else
        {
            printf("SPI open failed!\n");
        }
        gpioTerminate();
    }
    return 0;
}
