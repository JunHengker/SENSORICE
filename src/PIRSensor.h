#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H

#include <Arduino.h>
#include <constants.h>

class PIRSensor
{
public:
    PIRSensor(uint8_t pin, uint8_t buzzerPin);
    void setup();
    void handleMotion();
    static volatile bool motionDetected;

private:
    uint8_t _pin;
    uint8_t _buzzerPin;
};

#endif
