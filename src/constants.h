#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MACHINE_ID "sensorice"

#define DHT_PIN 4
#define DHT_TYPE DHT22
#define SOIL_SENSOR_PIN 36
#define PIR_PIN GPIO_NUM_12
#define BUZZER_PIN 15

#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883
#define MQTT_TOPIC_COMMAND "/SensoRice/command"
#define MQTT_TOPIC_RESPONSE "/SensoRice/response"

// Deep sleep constants
#define TIMESLEEP 15 // amount in seconds the ESP32 sleeps

#endif