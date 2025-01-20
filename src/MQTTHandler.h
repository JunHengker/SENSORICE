#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

#include <PubSubClient.h>
#include <WiFi.h>

class MQTTHandler
{
public:
    MQTTHandler(const char *server, uint16_t port, const char *topic);
    void setup(MQTT_CALLBACK_SIGNATURE);
    void setup();
    void ensureConnection();
    void loop();
    void publishMessage(const char *topic, const String &payload);

private:
    WiFiClient wifiClient;
    PubSubClient mqttClient;
    const char *server;
    uint16_t port;
    const char *topic;

    void reconnect();
};

#endif
