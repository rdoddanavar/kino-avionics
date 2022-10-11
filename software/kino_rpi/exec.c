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
#include <stdint.h>

// Project libraries
#include "pigpio.h"

//----------------------------------------------------------------------------//

// SPI setup
const uint8_t nByte = 4, nData = 9;

typedef union
{
    float value;
    uint8_t bytes[nByte];
} FloatUnion;

// General data
FloatUnion dataTime; // [s]

// BME280 data
FloatUnion dataTemp;     // [deg F]
FloatUnion dataPress;    // [psi]
FloatUnion dataAltPress; // [ft]
FloatUnion dataHum;      // [%]

// NEO6M data
FloatUnion dataLat;    // [deg]
FloatUnion dataLng;    // [deg]
FloatUnion dataAltGps; // [ft]
FloatUnion dataSpd;    // [ft/s]

FloatUnion *dataUnion[nData] = {&dataTime, &dataTemp,  &dataPress, &dataAltPress, &dataHum,
                               &dataLat,  &dataLng, &dataAltGps, &dataSpd};

const char dataKey[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8'};

//----------------------------------------------------------------------------//

int main(void)
{
    if (gpioInitialise() < 0)
    {
        printf("pigpio initialization failed\n");
    }
    else
    {
        
        printf("pigpio initialization successful\n");

        uint8_t  spiChan = 0, spiFlags = 0;
        uint16_t baud    = 32000; // Min: 32000, Max: 125000000

        int handle = spiOpen(spiChan, baud, spiFlags);

        char buf[nByte];
        uint8_t count = 1; // Number of bytes to read

        int status = 0;

        if (handle >= 0)
        {
            
            printf("SPI open successful!\n");

            uint16_t nSample = 10;
            uint16_t iSample;
            uint8_t  iData, iByte;

            float data[nSample][nData];

            for (iSample=0; iSample<nSample; iSample++)
            {  
                for (iData=0; iData<nData; iData++)
                {

                    buf[0] = dataKey[iData];
                    spiWrite(handle, buf, count);

                    for (iByte=0; iByte<nByte; iByte++)
                    {
                        status = spiRead(handle, buf, count);
                        dataUnion[iData]->bytes[iByte] = buf[0];
                    }

                    data[iSample][iData] = dataUnion[iData]->value;

                    //printf("test: %c, %.2f\n", dataKey[iData], dataUnion[iData]->value);

                }
                time_sleep(0.1);
            }

            spiClose(handle);

            for (iSample=0; iSample<nSample; iSample++)
            {
                
                printf("(%d/%d): ", iSample, nSample);
                
                for (iData=0; iData<nData; iData++)
                {
                    printf("%c%.3f, ", dataKey[iData], data[iSample][iData]);
                }

                printf("\n");

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
