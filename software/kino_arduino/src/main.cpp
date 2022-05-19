#include <Arduino.h>

void setup()
{

    // init Serial USB
    Serial.begin(115200);//for debugging  

}

void loop()
{ 
    dataOut = 5;
    Serial.println(dataOut);
    delay(500);
}