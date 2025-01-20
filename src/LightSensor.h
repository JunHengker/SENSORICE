#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include <Wire.h>
#include <BH1750FVI.h>
#include <constants.h>

class LightSensor
{
private:
    BH1750FVI bh1750;

public:
    LightSensor()
        : bh1750(BH1750_DEFAULT_I2CADDR, BH1750_ONE_TIME_HIGH_RES_MODE_2, BH1750_SENSITIVITY_DEFAULT, BH1750_ACCURACY_DEFAULT) {}

    void setup();
    void read();
    String sendToBackend();
    void powerDown();
    void powerOn();
    float getLightLevel();
};

#endif
