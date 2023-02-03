#include <Arduino.h>
#include <Wire.h>
#include <MPU9250.h>

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

    Serial.begin(9600);

    iSample = 0;
    nSample = (uint16_t) runTime*sampleRate;
    nDelay  = (uint16_t) (1.0f/sampleRate)*s_to_ms;

    Wire.begin();

    if (!mpu.setup(i2cAddress))
    {
        Serial.println("Could not find a valid MPU9250 sensor, check wiring!");
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

        if (mpu.available())
        {

            update_accel_gyro();
            update_mag();

            dataOut += "LinAcc: ";
            dataOut += String(mpu.getLinearAccX()) + ", ";
            dataOut += String(mpu.getLinearAccY()) + ", ";
            dataOut += String(mpu.getLinearAccZ()) + ", ";

            dataOut += "Gyro: ";
            dataOut += String(mpu.getGyroX()) + ", ";
            dataOut += String(mpu.getGyroY()) + ", ";
            dataOut += String(mpu.getGyroZ()) + ", ";

            dataOut += "Mag: ";
            dataOut += String(mpu.getMagX()) + ", ";
            dataOut += String(mpu.getMagY()) + ", ";
            dataOut += String(mpu.getMagZ());

        }

        uint16_t len = dataOut.length() + 1;
        char     buf[len];

        dataOut.toCharArray(buf, len);
        Serial.println(dataOut);    

        delay(nDelay);
        iSample++;

    }

}