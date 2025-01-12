#include "SoilSensor.h"

void SoilSensor::setup()
{
    pinMode(SOIL_SENSOR_PIN, INPUT);
    while (isnan(analogRead(SOIL_SENSOR_PIN)))
    {
        Serial.println(F("[ERROR] Soil Moisture Sensor is not detected"));
        delay(5000);
    }
    Serial.println(F("[INFO] Soil Moisture Sensor initialized"));
}

void SoilSensor::read()
{
    float soilMoisture = (float(analogRead(SOIL_SENSOR_PIN)) / 1023.0) * 3.3;
    if (isnan(soilMoisture))
    {
        Serial.println("[ERROR] Soil Moisture Sensor is not connected");
        return;
    }

    Serial.print("Soil Moisture Sensor Voltage: ");
    Serial.print(soilMoisture);
    Serial.println(" V");
}

// todo: send to backend
String SoilSensor::sendToBackend()
{
    float soilMoisture = (float(analogRead(SOIL_SENSOR_PIN)) / 1023.0) * 3.3;
    if (isnan(soilMoisture))
    {
        Serial.println("[ERROR] Soil Moisture Sensor is not connected");
        String payload = "Error reading soil moisture";
        return payload;
    }

    String payload = "{\"machine_id\":\"" + String(MACHINE_ID) + "\",\"soil_moisture\":" + String(soilMoisture) + "}";
    return payload;
}