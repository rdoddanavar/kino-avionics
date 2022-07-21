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

//----------------------------------------------------------------------------//

FloatUnion dataTime;  // [s]
FloatUnion dataTemp;  // [deg C]
FloatUnion dataPress; // [hPa]
FloatUnion dataAlt;   // [m]
FloatUnion dataHum;   // [%]

FloatUnion *dataUnion[] = {&dataTime, &dataTemp, &dataPress, &dataAlt, &dataHum};
const char  dataKey[]   = {'t', 'm', 'p', 'a', 'h'};

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

        unsigned int spiChan  = 0;
        unsigned int baud     = 32000; // Min: 32000, Max: 125000000
        unsigned int spiFlags = 0;

        int handle = spiOpen(spiChan, baud, spiFlags);

        char buf[nByte];
        unsigned int count = 1; // Number of bytes to read

        int status = 0;

        if (handle >= 0)
        {
            
            printf("SPI open successful!\n");

            const unsigned int nCount = 10;
            const unsigned int nData  = 5;

            int iPing;
            int iData;
            int iByte;

            float data[nCount][nData];

            for (iPing=0; iPing<nCount; iPing++)
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

                    data[iPing][iData] = dataUnion[iData]->value;

                    //printf("test: %c, %.2f\n", dataKey[iData], dataUnion[iData]->value);

                }
                time_sleep(0.1);
            }

            spiClose(handle);

            for (iPing=0; iPing<nCount; iPing++)
            {
                
                printf("(%d/%d): ", iPing, nCount);
                
                for (iData=0; iData<nData; iData++)
                {
                    printf("%c%.3f, ", dataKey[iData], data[iPing][iData]);
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
