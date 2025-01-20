
#include <Arduino.h>
#include <constants.h>

#define TIMEFACTOR 1000000 // factor between seconds and microseconds

void initSleep(uint64_t timeSleep)
{
    Serial.println(F("Preparing deep sleep now"));

    Serial.println("Set timer for sleep-wakeup every " + String(timeSleep) + " seconds");
    esp_sleep_enable_timer_wakeup(timeSleep * TIMEFACTOR);

    // biarin ulp jalan di deep sleep
    esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
    Serial.println(F("Going into deep sleep now"));
    esp_deep_sleep_start();
}