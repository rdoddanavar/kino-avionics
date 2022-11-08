#include <Arduino.h>
#include <Wire.h>
#include <MPU9250.h>

#include <unity.h> // Unit testing only

//----------------------------------------------------------------------------//

// General setup
const float runTime    = 60.0f;  // [s]
const float sampleRate = 1.0f;   // [Hz]
const float s_to_ms    = 1.0e3f;

uint16_t iSample;
uint16_t nSample;
uint16_t nDelay; // [ms]

String dataOut;

//----------------------------------------------------------------------------//

// MPU9250 setup
const uint8_t i2cAddress = 0x68;
const float   magDecl    = 9.43f; // [deg]

MPU9250 mpu;

//----------------------------------------------------------------------------//

void setup()
{

    iSample = 0;
    nSample = (uint16_t) runTime*sampleRate;
    nDelay  = (uint16_t) (1.0f/sampleRate)*s_to_ms;
    
    UNITY_BEGIN();

    Wire.begin();

    if (!mpu.setup(i2cAddress))
    {
        TEST_MESSAGE("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

    mpu.setMagneticDeclination(magDecl);
    mpu.calibrateAccelGyro();
    mpu.calibrateMag();

}

//----------------------------------------------------------------------------//

void loop()
{

    if (iSample < nSample)
    {   
        
        dataOut = "";

        if (mpu.update())
        {
            
            dataOut += "LinAcc: ";
            dataOut += String(mpu.getLinearAccX()) + ", ";
            dataOut += String(mpu.getLinearAccY()) + ", ";
            dataOut += String(mpu.getLinearAccZ()) + ", ";

            dataOut += "Gyro: ";
            dataOut += String(mpu.getGyroX()) + ", ";
            dataOut += String(mpu.getGyroY()) + ", ";
            dataOut += String(mpu.getGyroZ()) + ", ";

            dataOut += "Euler: ";
            dataOut += String(mpu.getEulerX()) + ", ";
            dataOut += String(mpu.getEulerY()) + ", ";
            dataOut += String(mpu.getEulerZ()) + ", ";

        }
        
        uint16_t len = dataOut.length() + 1;
        char     buf[len];

        dataOut.toCharArray(buf, len);
        TEST_MESSAGE(buf);

        delay(nDelay);
        iSample++;

    }
    else
    {
        UNITY_END();
    }

}