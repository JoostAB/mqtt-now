/**
 * @file main.cpp
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */
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
