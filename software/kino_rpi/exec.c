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
#include <stdbool.h>

// Project libraries
#include "pigpio.h"

// Types
const int nByte = 4;

typedef union
{
    float value;
    uint8_t bytes[nByte];
} FloatUnion;

FloatUnion time; // [s]

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
        unsigned int count = nByte; // Number of bytes to read

        int status = 0;

        if (handle >= 0)
        {
            
            printf("SPI open successful!\n");

            for (int iPing=0; iPing<50; iPing++)
            {
                
                status = spiRead(handle, buf, count);

                for (int iByte=0; iByte<nByte; iByte++)
                {
                    time.bytes[iByte] = buf[iByte];
                }

                printf("Time: %.3f\n", time.value);

            }

            spiClose(handle);

        }
        else
        {
            printf("SPI open failed!\n");
        }
        gpioTerminate();
    }
    return 0;
}
