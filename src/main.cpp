#include <Arduino.h>
#include "constants.h"
#include "LightSensor.h"
#include "SoilSensor.h"
#include "DHTSensor.h"
#include "PIRSensor.h"
#include "WiFiHandler.h"
#include "MQTTHandler.h"
#include "MessageHandler.h"
#include <ArduinoJson.h>
#include <Sleep.hpp>
#include "driver/rtc_io.h"

// WiFi and MQTT
WiFiHandler wifiHandler("SensoRiceAP");
MQTTHandler mqttHandler(MQTT_SERVER, MQTT_PORT, MQTT_TOPIC_COMMAND);

JsonDocument doc;

// Deepsleep attr
RTC_DATA_ATTR bool firstBoot = true;

void setup()
{
  Serial.begin(9600); // kalo deepsleep gagal, comment ini
  Serial.println(F("[INFO] Starting Paddy Field Monitoring System..."));

  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0 || wakeup_reason == ESP_SLEEP_WAKEUP_TIMER || wakeup_reason == ESP_SLEEP_WAKEUP_ULP || firstBoot)
  {

    if (firstBoot || wakeup_reason == ESP_SLEEP_WAKEUP_TIMER || wakeup_reason == ESP_SLEEP_WAKEUP_ULP)
    {
      // Sensor instances
      LightSensor lightSensor;
      SoilSensor soilSensor;
      DHTSensor dhtSensor;

      // Setup WiFi and MQTT
      wifiHandler.setupWiFi();
      mqttHandler.setup();

      // Sensor setup
      lightSensor.setup();
      soilSensor.setup();
      dhtSensor.setup();

      Serial.println(F("[INFO] System setup complete"));

      // karena jun udah bikin satu satu, kita buat satu satu

      doc.clear();
      doc["machine_id"] = MACHINE_ID;
      doc["light_level"] = lightSensor.getLightLevel();
      mqttHandler.publishMessage(MQTT_TOPIC_RESPONSE, doc.as<String>());

      doc.remove("light_level");

      doc["soil_moisture"] = soilSensor.getSoilMoisture();
      mqttHandler.publishMessage(MQTT_TOPIC_RESPONSE, doc.as<String>());

      doc.remove("soil_moisture");

      doc["temperature"] = dhtSensor.getTemperature();
      mqttHandler.publishMessage(MQTT_TOPIC_RESPONSE, doc.as<String>());

      doc.remove("temperature");

      doc["humidity"] = dhtSensor.getHumidity();
      mqttHandler.publishMessage(MQTT_TOPIC_RESPONSE, doc.as<String>());
    }

    if (wakeup_reason == ESP_SLEEP_WAKEUP_EXT0)
    {
      PIRSensor pirSensor(PIR_PIN, BUZZER_PIN);
      pirSensor.handleMotion();

      // Setup WiFi and MQTT
      wifiHandler.setupWiFi();
      mqttHandler.setup();

      doc.clear();
      doc["machine_id"] = MACHINE_ID;
      doc["motion"] = "detected";
      mqttHandler.publishMessage(MQTT_TOPIC_RESPONSE, doc.as<String>());
    }
  }

  // close serial before deep sleep
  Serial.end(); // kalo deepsleep gagal, comment ini

  if (firstBoot)
  {
    firstBoot = false;
  }

  // set PIR as wakeup source
  esp_sleep_enable_ext0_wakeup(PIR_PIN, 1);
  rtc_gpio_pullup_dis(PIR_PIN);
  rtc_gpio_pulldown_en(PIR_PIN);

  // deep sleep for TIMESLEEP seconds
  initSleep(TIMESLEEP);
}

void loop()
{
}