#include <Arduino.h>

#define MQTT_HOST "192.168.0.10"
#define MQTTNOW_DEBUG 1
#include <mqtt-now.h>

MqttNow mqttNow;

void setup() {
  mqttNow.begin();
}

void loop() {
  mqttNow.update();
}
