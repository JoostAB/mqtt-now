/**
 * @file mqtt-now-slave.cpp
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <mqtt-now-slave.h>
#if !(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))

MqttNowSlave::MqttNowSlave() : MqttNowNode() {};

void MqttNowSlave::begin() {
  MqttNowNode::begin();
};
void MqttNowSlave::update() {
  MqttNowNode::update();
};

#endif // !(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))