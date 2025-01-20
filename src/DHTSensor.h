#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <DHT.h>
#include <DHT_U.h>
#include <constants.h>

class DHTSensor
{
private:
    DHT dht;

public:
    DHTSensor() : dht(DHT_PIN, DHT_TYPE) {}

    void setup();
    void read();
    String sendTempToBackend();
    String sendHumidityToBackend();
    float getTemperature();
    float getHumidity();
};

#endif
