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

        char buf[1];
	const int length = 12;
	char message[length];
        unsigned int count = 1;

        int status = 0;

        if (handle >= 0)
        {
            printf("SPI open successful!\n");

            for (int i=0; i<length; i++)
            {
                status = spiRead(handle, buf, count);
		printf("%c", buf[0]);
            }

	    printf("\n");
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
