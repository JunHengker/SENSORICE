#include <Arduino.h>
#include "constants.h"
#include "LightSensor.h"
#include "SoilSensor.h"
#include "DHTSensor.h"
#include "PIRSensor.h"
#include <WiFiManager.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient wifi;
PubSubClient client(wifi);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

WiFiManager wm;

LightSensor lightSensor;
SoilSensor soilSensor;
DHTSensor dhtSensor;
PIRSensor pirSensor(PIR_PIN, BUZZER_PIN);

bool isMsgValid(String msg)
{
    if (msg.length() < 3)
    {
        return false;
    }

    String machineId = msg.substring(0, 6);
    if (machineId != MACHINE_ID)
    {
        return false;
    }

    return true;
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void callbackMsg(char *topic, byte *payload, uint16_t length)
{
    // convert c string to String
    String data = "";
    for (int i = 0; i < length; i++)
    {
        data += (char)payload[i];
    }

    Serial.print(F("[MQTT:INFO] Message arrived ["));
    Serial.println(data);

    if (!isMsgValid(data))
    {
        Serial.println(F("[MQTT:ERROR] Invalid message"));
        return;
    }

    String command = getValue(data, '#', 1);
    String value = getValue(data, '#', 2);

    Serial.println(F("[MQTT:INFO] Command: "));
    Serial.println(command);
    Serial.println(F("[MQTT:INFO] Value: "));
    Serial.println(value);

    // command handling
    if (command == "light")
    {
    }
    else if (command == "soil")
    {
    }
    else if (command == "temp")
    {
    }
    else if (command == "hum")
    {
    }
    else if (command == "motion")
    {
    }
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print(F("[MQTT:INFO] Connecting to MQTT broker..."));
        // create random client ID
        String clientId = "SensoRice-";
        clientId += String(random(0xffff), HEX);
        // attempt to connect
        if (client.connect(clientId.c_str()))
        {
            Serial.println(F("connected"));
            client.subscribe(MQTT_TOPIC_COMMAND);
        }
        else
        {
            Serial.print(F("failed, rc="));
            Serial.print(client.state());
            Serial.println(F(" try again in 5 seconds"));
            delay(5000);
        }

        // Check if WiFi is connected
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println(F("[WiFi:ERROR] Reconnecting to WiFi..."));
            wm.autoConnect("SensoRice");

            if (WiFi.status() == WL_CONNECTED)
            {
                Serial.println(F("[WiFi:INFO] WiFi connected"));
            }
            else
            {
                Serial.println(F("[WiFi:ERROR] WiFi connection failed"));
            }
        }
    }
}

void setup()
{
    Serial.begin(9600);
    Serial.println();
    Serial.println(F("[INFO] Starting Paddy Field Monitoring System..."));
    Serial.println(F("[INFO] Setting up WIFI Manager..."));
    wm.setDebugOutput(true);
    WiFi.mode(WIFI_STA);
    wm.setConfigPortalTimeout(180);

    bool res = wm.autoConnect("SensoRice");
    if (!res)
    {
        Serial.println(F("[WiFi:ERROR] Failed to connect to WiFi"));
        ESP.restart();
    }
    else
    {
        Serial.println(F("[WiFi:INFO] WiFi connected"));
    }

    Serial.println(F("[INFO] Setting up MQTT client..."));
    client.setServer(MQTT_SERVER, MQTT_PORT);
    client.setCallback(callbackMsg);

    lightSensor.setup();
    soilSensor.setup();
    dhtSensor.setup();
    pirSensor.setup();
    Serial.println(F("[INFO] Setup completed"));
}

void loop()
{
    if (!client.connected())
    {
        reconnect();
    }

    client.loop();

    unsigned long now = millis();
    if (now - lastMsg > 5000)
    {
        lastMsg = now;
        lightSensor.read();
        soilSensor.read();
        dhtSensor.read();
        if (PIRSensor::motionDetected)
        {
            pirSensor.handleMotion();
        }
    }
}
