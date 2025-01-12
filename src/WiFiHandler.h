#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <WiFiManager.h>
#include <WiFi.h>

class WiFiHandler
{
public:
    WiFiHandler(const char *apName);
    void setupWiFi();
    void ensureConnection();

private:
    WiFiManager wm;
    const char *apName;
};

#endif
