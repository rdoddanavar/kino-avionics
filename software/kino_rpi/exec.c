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

// General setup
const float sampleRate = 1.0f; // [Hz]

// SPI setup
#define NBYTE (4)
//#define NDATA (9)
#define NDATA (5)

typedef union
{
    float value;
    uint8_t bytes[NBYTE];
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

// FloatUnion *dataOut[NDATA] = {&dataTime, &dataTemp,  &dataPress, &dataAltPress, &dataHum,
//                                 &dataLat, &dataLng, &dataAltGps, &dataSpd};

FloatUnion *dataOut[NDATA] = {&dataTime, &dataTemp,  &dataPress, &dataAltPress, &dataHum};

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
        uint32_t baud    = 32000; // Min: 32000, Max: 125000000

        int handle = spiOpen(spiChan, baud, spiFlags);

        char buf[NBYTE];
        uint8_t count = 1; // Number of bytes to read

        int status = 0;

        if (handle >= 0)
        {
            
            printf("SPI open successful!\n");

            uint16_t nSample = 100;
            uint16_t iSample;
            uint8_t  iData, iByte;

            float data[nSample][NDATA];

            // Read data using SPI

            for (iSample=0; iSample<nSample; iSample++)
            {  
                for (iData=0; iData<NDATA; iData++)
                {

                    buf[0] = (char) (iData + 48);
                    spiWrite(handle, buf, count);

                    for (iByte=0; iByte<NBYTE; iByte++)
                    {
                        status = spiRead(handle, buf, count);
                        dataOut[iData]->bytes[iByte] = buf[0];
                    }

                    data[iSample][iData] = dataOut[iData]->value;

                    printf("%.3f, ", dataOut[iData]->value);

                }
                printf("\n");
                time_sleep(1.0f/sampleRate);
            }

            spiClose(handle);

            // Write data to file
            FILE *fileOut;

            fileOut = fopen("data.csv", "w+");

            if (fileOut != NULL)
            {
                
                char headerStr[] = "dataTime, dataTemp, dataPress, dataAltPress, dataHum, dataLat, dataLng, dataAltGps, dataSpd";
                fprintf(fileOut, "%s\n", headerStr);
                
                for (iSample=0; iSample<nSample; iSample++)
                {
                    
                    for (iData=0; iData<NDATA; iData++)
                    {
                        fprintf(fileOut, "%.3f, ", data[iSample][iData]);
                    }

                    fprintf(fileOut, "\n");

                }

                fclose(fileOut);

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
