#include "DHTSensor.h"

void DHTSensor::setup()
{
    dht.begin();
    while (isnan(dht.readTemperature()) || isnan(dht.readHumidity()))
    {
        Serial.println(F("[ERROR] DHT22 Sensor is not detected"));
        delay(5000);
    }
    Serial.println(F("[INFO] DHT22 Sensor initialized"));
}

void DHTSensor::read()
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature))
    {
        Serial.println(F("[ERROR] Failed to read from DHT22 sensor"));
        return;
    }

    Serial.print(F("Humidity: "));
    Serial.print(humidity);
    Serial.println(F(" %"));

    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.println(F(" °C"));
}

// todo: send to backend
String DHTSensor::sendTempToBackend()
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature))
    {
        Serial.println(F("[ERROR] Failed to read from DHT22 sensor"));
        String payload = "Error reading DHT22 sensor";
        return payload;
    }

    String payload = "{\"machine_id\":\"" + String(MACHINE_ID) + "\",\"temperature\":" + String(temperature) + "}";
    return payload;
}

String DHTSensor::sendHumidityToBackend()
{
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature))
    {
        Serial.println(F("[ERROR] Failed to read from DHT22 sensor"));
        String payload = "Error reading DHT22 sensor";
        return payload;
    }

    String payload = "{\"machine_id\":\"" + String(MACHINE_ID) + "\",\"humidity\":" + String(humidity) + "}";
    return payload;
}