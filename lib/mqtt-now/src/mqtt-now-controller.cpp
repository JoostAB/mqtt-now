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
#if defined(MQTT_NOW_CONTROLLER) | defined(MQTT_TEST_COMPILE)

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

void MqttNowController::messageReceived(const uint8_t *macFrom, uint8_t type, msg_base *msg, uint8_t len) {

}

#endif // MQTT_NOW_CONTROLLER