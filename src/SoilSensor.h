#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H

#include <Arduino.h>
#include <constants.h>

class SoilSensor
{
public:
    void setup();
    void read();
    String sendToBackend();
};

#endif
