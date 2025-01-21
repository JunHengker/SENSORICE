#include "MQTTHandler.h"

MQTTHandler::MQTTHandler(const char *server, uint16_t port, const char *topic)
    : mqttClient(wifiClient), server(server), port(port), topic(topic) {}

void MQTTHandler::setup(MQTT_CALLBACK_SIGNATURE)
{
    mqttClient.setServer(server, port);
    mqttClient.setCallback(callback);
    Serial.println(F("[INFO] MQTT setup completed"));
}

void MQTTHandler::setup()
{
    mqttClient.setServer(server, port);
    Serial.println(F("[INFO] MQTT setup completed"));

    reconnect();
}

void MQTTHandler::ensureConnection()
{
    if (!mqttClient.connected())
    {
        reconnect();
    }
}

void MQTTHandler::reconnect()
{
    while (!mqttClient.connected())
    {
        Serial.print(F("[MQTT:INFO] Connecting to MQTT broker..."));
        String clientId = "SensoRice-";
        clientId += String(random(0xffff), HEX);

        if (mqttClient.connect(clientId.c_str()))
        {
            Serial.println(F("connected"));
            mqttClient.subscribe(topic);
        }
        else
        {
            Serial.print(F("failed, rc="));
            Serial.println(mqttClient.state());
            delay(5000);
        }
    }
}

void MQTTHandler::loop()
{
    mqttClient.loop();
}

void MQTTHandler::publishMessage(const char *topic, const String &payload)
{
    mqttClient.publish(topic, payload.c_str());
    Serial.printf("[MQTT:INFO] Message published to topic '%s': %s\n", topic, payload.c_str());
}
