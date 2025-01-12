// Constants
#include <constants.h>
#include <Wire.h>
// BH1750 Breakout Light Intensity Sensor
#include <BH1750FVI.h>
// DHT22 Temperature and Humidity Sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
// Wifi Manager
#include <WiFiManager.h>
#include <WiFi.h>
// MQTT
#include <PubSubClient.h>

BH1750FVI myBH1750(BH1750_DEFAULT_I2CADDR, BH1750_ONE_TIME_HIGH_RES_MODE_2, BH1750_SENSITIVITY_DEFAULT, BH1750_ACCURACY_DEFAULT);
DHT dht(DHT_PIN, DHT_TYPE);

// light sensor functions
void setupLightSensor()
{
  while (myBH1750.begin() != true)
  {
    Serial.println(F(" [ERROR] Breakout Light Intensity Sensor is not detected"));
    delay(5000);
  }

  // change BH1750 settings on the fly
  myBH1750.setCalibration(1.06);
  myBH1750.setSensitivity(1.00);
  myBH1750.setResolution(BH1750_ONE_TIME_HIGH_RES_MODE_2);
  Serial.println(F("[INFO] Breakout Light Intensity Sensor initialized"));
}

void LightSensor()
{
  float lightLevel = myBH1750.readLightLevel();

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

  Serial.print(F("Resolution value....: 0x"));
  Serial.println(myBH1750.getResolution(), HEX);

  Serial.print(F("Calibration value...: "));
  Serial.println(myBH1750.getCalibration());

  Serial.print(F("Sensitivity value...: "));
  Serial.println(myBH1750.getSensitivity());

  // Measure light levels on backend
}

void powerDownLightSensor()
{
  myBH1750.powerDown(); // Sleep mode (1uA)
}

// soil sensor functions
void setupSoilSensor()
{
  pinMode(SOIL_SENSOR_PIN, INPUT);
  while (isnan(analogRead(SOIL_SENSOR_PIN)))
  {
    Serial.println(F("[ERROR] Soil Moisture Sensor is not detected"));
    delay(5000);
  }
  Serial.println(F("[INFO] Soil Moisture Sensor initialized"));
}

void SoilSensor()
{
  float soilMoisture = (float(analogRead(SOIL_SENSOR_PIN)) / 1023.0) * 3.3; // read sensor
  // check if sensor is connected
  if (isnan(soilMoisture))
  {
    Serial.println("Soil Moisture Sensor is not connected");
    return;
  }

  Serial.print("Soil Moisture Sensor Voltage: ");
  Serial.print(soilMoisture);
  Serial.println(" V");
  // Serial.println("Soil Moisture: ");

  // Measure soil moisture (dry or not on backend) on backend
}

// DHT22 sensor functions
void setupDHTSensor()
{
  dht.begin(); // Initialize DHT sensor
  while (isnan(dht.readTemperature()) || isnan(dht.readHumidity()))
  {
    Serial.println(F("[ERROR] DHT22 Sensor is not detected"));
    delay(5000);
  }
  Serial.println(F("[INFO] DHT22 Sensor initialized"));
}

void DHTSensor()
{
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and retry
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

// PIR motion sensor functions
void setupPIRSensor()
{
  pinMode(PIR_PIN, INPUT);
  while (isnan(digitalRead(PIR_PIN)))
  {
    Serial.println(F("[ERROR] PIR Motion Sensor is not detected"));
    delay(5000);
  }
  Serial.println(F("[INFO] PIR Motion Sensor initialized"));
}

void PIRSensor()
{
  int motionDetected = digitalRead(PIR_PIN);

  if (motionDetected == HIGH)
  {
    Serial.println(F("[ALERT] Motion detected!"));
    // Send alert to backend
    digitalWrite(BUZZER_PIN, HIGH);
  }
  else
  {
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println(F("[INFO] No motion detected"));
  }
}

void setup()
{

  Serial.begin(9600);
  Serial.println();
  Serial.println("[INFO] Starting Paddy Field Monitoring System...");
  Serial.println("[INFO] Setting up environment sensors...");
  setupLightSensor();
  setupSoilSensor();
  setupDHTSensor();
  setupPIRSensor();
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  LightSensor();
  SoilSensor();
  DHTSensor();
  PIRSensor();
  powerDownLightSensor();
  myBH1750.powerOn();
  myBH1750.reset();
  delay(1500);
}