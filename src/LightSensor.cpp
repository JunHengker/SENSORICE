#include "LightSensor.h"

void LightSensor::setup()
{
    while (!bh1750.begin())
    {
        Serial.println(F("[ERROR] Breakout Light Intensity Sensor is not detected"));
        delay(5000);
    }

    bh1750.setCalibration(1.06);
    bh1750.setSensitivity(1.00);
    bh1750.setResolution(BH1750_ONE_TIME_HIGH_RES_MODE_2);
    Serial.println(F("[INFO] Breakout Light Intensity Sensor initialized"));
}

void LightSensor::read()
{
    float lightLevel = bh1750.readLightLevel();

    Serial.println();
    Serial.print(F("Light level.........: "));
    if (lightLevel != BH1750_ERROR)
    {
        Serial.print(lightLevel, 2);
        Serial.println(F(" lux"));
    }
    else
    {
        Serial.println(F("error"));
    }
}

// todo : send to backend
String LightSensor::sendToBackend()
{
    float lightLevel = bh1750.readLightLevel();

    if (lightLevel == BH1750_ERROR)
    {
        Serial.println(F("[ERROR] Breakout Light Intensity Sensor is not connected"));
        String payload = "Error reading light level";
        return payload;
    }

    String payload = "{\"machine_id\":\"" + String(MACHINE_ID) + "\",\"light_level\":" + String(lightLevel) + "}";
    return payload;
}

void LightSensor::powerDown()
{
    bh1750.powerDown();
}

void LightSensor::powerOn()
{
    bh1750.powerOn();
    bh1750.reset();
}
