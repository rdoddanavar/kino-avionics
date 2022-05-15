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
        unsigned int baud     = 9600;
        unsigned int spiFlags = 0;

        int handle = spiOpen(spiChan, baud, spiFlags);

        if (handle >= 0)
        {
            printf("SPI open successful!\n");
        }
        else
        {
            printf("SPI open failed!\n");
        }
        gpioTerminate();
    }
    return 0;
}