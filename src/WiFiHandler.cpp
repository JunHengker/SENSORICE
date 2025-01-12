#include "WiFiHandler.h"

WiFiHandler::WiFiHandler(const char *apName) : apName(apName) {}

void WiFiHandler::setupWiFi()
{
    Serial.println(F("[INFO] Setting up WiFi Manager..."));
    wm.setDebugOutput(true);
    WiFi.mode(WIFI_STA);
    wm.setConfigPortalTimeout(180);

    bool res = wm.autoConnect(apName);
    if (!res)
    {
        Serial.println(F("[WiFi:ERROR] Failed to connect to WiFi"));
        ESP.restart();
    }
    else
    {
        Serial.println(F("[WiFi:INFO] WiFi connected"));
    }
}

void WiFiHandler::ensureConnection()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(F("[WiFi:ERROR] Reconnecting to WiFi..."));
        wm.autoConnect(apName);

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
