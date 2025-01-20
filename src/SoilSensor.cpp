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
    // Read the analog value from the soil moisture sensor pin
    int soilMoisture = analogRead(SOIL_SENSOR_PIN);

    //
    if (isnan(soilMoisture))
    {
        Serial.println(F("[ERROR] Soil Moisture Sensor is not connected"));
        return "";
    }

    // Send the soil moisture value to the backend
    String payload = "{\"machine_id\":\"" + String(MACHINE_ID) + "\",\"soil_moisture\":" + String(soilMoisture) + "}";
    Serial.print(F("[MQTT:ACTION] Sending soil moisture data to backend: "));
    Serial.println(payload);
    return payload;

    // Explanation
    // Wet (1500–1700): High moisture, typically during irrigation or after rain.
    // Optimal (1700–2000): Best range for paddy field growth.
    // Moderate (2000–2200): Slightly dry, may require irrigation soon.
    // Dry (2200–2500): Low moisture, urgent irrigation needed.
    // Unknown: Values outside the expected range (sensor malfunction or error).
}
