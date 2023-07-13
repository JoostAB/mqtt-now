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
  if (type == msgTypeIntro) {
    msg_intro* _msg_intro = (msg_intro*)msg;
    if (_msg_intro->network_uuid == network_uuid) {
      // Correct Network ID. Add to network and respond with welcome
    } else {
      // Wrong Network ID. reject!
      msg_error err;
      getMessageStruct(msgTypeError, &err);
      strcpy(err.error_msg, "Invalid network ID");
      sendMessage(msgTypeError, &err, macFrom);
      return;
    }
  }

}

#endif // MQTT_NOW_CONTROLLER