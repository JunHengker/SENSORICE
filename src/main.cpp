#include <Arduino.h>
#include "constants.h"
#include "LightSensor.h"
#include "SoilSensor.h"
#include "DHTSensor.h"
#include "PIRSensor.h"
#include "WiFiHandler.h"
#include "MQTTHandler.h"
#include "MessageHandler.h"

// Sensor instances
LightSensor lightSensor;
SoilSensor soilSensor;
DHTSensor dhtSensor;
PIRSensor pirSensor(PIR_PIN, BUZZER_PIN);

// WiFi and MQTT
WiFiHandler wifiHandler("SensoRiceAP");
MQTTHandler mqttHandler(MQTT_SERVER, MQTT_PORT, MQTT_TOPIC_COMMAND);

void mqttCallback(char *topic, byte *payload, uint16_t length)
{
  String data = "";
  for (int i = 0; i < length; i++)
  {
    data += (char)payload[i];
  }

  Serial.printf("[MQTT:INFO] Message received on topic '%s': %s\n", topic, data.c_str());

  // Validate the message
  if (!MessageHandler::isMessageValid(data, MACHINE_ID))
  {
    Serial.println(F("[MQTT:ERROR] Invalid message"));
    return;
  }

  // Parse command and value from the message
  String command = MessageHandler::getValue(data, '#', 1);
  String value = MessageHandler::getValue(data, '#', 2);

  Serial.printf("[MQTT:INFO] Command: %s, Value: %s\n", command.c_str(), value.c_str());

  // Handle the command
  if (command == "light")
  {
    Serial.println(F("[MQTT:ACTION] Handling light command..."));
    String payload = lightSensor.sendToBackend();
    mqttHandler.publishMessage(MQTT_TOPIC_RESPONSE, payload);
  }
  else if (command == "soil")
  {
    Serial.println(F("[MQTT:ACTION] Handling soil command..."));
    String payload = soilSensor.sendToBackend();
    mqttHandler.publishMessage(MQTT_TOPIC_RESPONSE, payload);
  }
  else if (command == "temp")
  {
    Serial.println(F("[MQTT:ACTION] Handling temperature command..."));
    String payload = dhtSensor.sendTempToBackend();
    mqttHandler.publishMessage(MQTT_TOPIC_RESPONSE, payload);
  }
  else if (command == "hum")
  {
    Serial.println(F("[MQTT:ACTION] Handling humidity command..."));
    String payload = dhtSensor.sendHumidityToBackend();
    mqttHandler.publishMessage(MQTT_TOPIC_RESPONSE, payload);
  }
  else
  {
    Serial.println(F("[MQTT:ERROR] Unknown command"));
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println(F("[INFO] Starting Paddy Field Monitoring System..."));

  // Setup WiFi and MQTT
  wifiHandler.setupWiFi();
  mqttHandler.setup(mqttCallback);

  // Sensor setup
  lightSensor.setup();
  soilSensor.setup();
  dhtSensor.setup();
  pirSensor.setup();

  Serial.println(F("[INFO] System setup complete"));
}

void loop()
{
  // Ensure WiFi and MQTT are connected
  wifiHandler.ensureConnection();
  mqttHandler.ensureConnection();

  mqttHandler.loop();
  if (PIRSensor::motionDetected)
  {
    pirSensor.handleMotion();
    String payload = "{\"machine_id\":\"" + String(MACHINE_ID) + "\",\"motion\":\"detected\"}";
    mqttHandler.publishMessage(MQTT_TOPIC_RESPONSE, payload);
  }

  delay(2000);
}
