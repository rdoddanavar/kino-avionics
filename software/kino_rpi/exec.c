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
    printf("Hello world\n");

    if (gpioInitialise() < 0)
    {
        printf("pigpio initialization failed\n");
    }
    else
    {
        printf("pigpio initialization successful\n");

        gpioSetMode(18, PI_OUTPUT); // Set GPIO18 as output
        float delay = 0.5;

        for (int i = 0; i < 10; i++)
        {

            gpioWrite(18, 1); // Set GPIO18 high
            time_sleep(delay);
            gpioWrite(18, 0); // Set GPIO18 low
            time_sleep(delay);

        }
        gpioTerminate();
    }
    return 0;
}