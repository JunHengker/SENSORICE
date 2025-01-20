#include "PIRSensor.h"

// volatile bool PIRSensor::motionDetected = false;

PIRSensor::PIRSensor(uint8_t pin, uint8_t buzzerPin) : _pin(pin), _buzzerPin(buzzerPin) {}

void PIRSensor::setup()
{
    // pinMode(_pin, INPUT);
    pinMode(_buzzerPin, OUTPUT);
    // attachInterrupt(digitalPinToInterrupt(_pin), []()
    //                 { PIRSensor::motionDetected = true; }, RISING);
}

void PIRSensor::handleMotion()
{
    // if (motionDetected)
    // {
    // motionDetected = false; // Clear the flag
    Serial.println(F("[ALERT] Motion detected!"));
    digitalWrite(_buzzerPin, HIGH);
    delay(1000); // Buzzer sound duration
    digitalWrite(_buzzerPin, LOW);
    // }
}