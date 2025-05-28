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
  #ifdef MQTT_NOW_BRIDGE
  MqttNowBridge::begin();
  #endif
  MqttNowNode::begin();
};

void MqttNowController::update() {
  #ifdef MQTT_NOW_BRIDGE
  MqttNowBridge::update();
  #endif
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

#ifdef MQTT_NOW_BRIDGE
result_t MqttNowController::_doAction(char act) {
  PRINTLNS("On controller:");
  switch (act) {
    case MSG_ACTIONREC:
      PRINTLNS("Returned message received");
      return _handleBridgeMessage();
    case MSG_ACTIONSUB:
      PRINTLNS("Subscribe command received");
      //return _handleSubscribe();
      break;
    case MSG_ACTIONUNS:
      PRINTLNS("Unsubscribe command received");
      //return _handleUnsubscribe();
      break;
    case MSG_ACTIONPUB:
      PRINTLNS("Publish command received");
      //return _handlePublish();
      break;
    case MSG_ACTIONRBT:
      PRINTLNS("Reboot command received");
      return _handleReboot();
      
    default:
      PRINTLNS("Unknown command received");
      return result_error;
  }
  return result_success;
}

result_t MqttNowController::_handleReboot() {
  delay(500);
  ESP.restart();
  return result_success;
}

result_t MqttNowController::_handleBridgeMessage() {
  // Strip first 4 characters from buffer
  _comBuff = _comBuff.substring(4);
  // Check if combuff starts with MSG_START
  if (_comBuff.startsWith(MSG_START)) {
    PRINTF("Resulting combuff (%s) starting with MSG_START, send to client\n",_comBuff.c_str());
    return sendSerial(_comBuff.c_str());
  }
  // Resulting string is not a command, so check if it is a returned MQTT message

  return result_error;
}

#endif
#endif // MQTT_NOW_CONTROLLER