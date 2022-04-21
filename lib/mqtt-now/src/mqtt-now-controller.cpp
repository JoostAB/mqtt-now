/**
 * @file mqtt-now-controller.cpp
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <mqtt-now-controller.h>
#ifdef MQTT_NOW_CONTROLLER

MqttNowController::MqttNowController() : MqttNowNode() {};

void MqttNowController::begin() {
  MqttNowNode::begin();
  if (!COM) {
    COM.begin(SERIALBAUDRATE);
  }
};

void MqttNowController::update() {
  MqttNowNode::update();
};

#endif // MQTT_NOW_CONTROLLER