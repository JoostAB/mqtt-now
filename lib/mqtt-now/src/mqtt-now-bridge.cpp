/**
 * @file mqtt-now-bridge.cpp
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 1.0
 * @date 2023-12-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <mqtt-now-bridge.h>
#if defined(MQTT_NOW_CONTROLLER) | defined(MQTT_NOW_CLIENT) | defined(MQTT_TEST_COMPILE)

MqttNowBridge::MqttNowBridge() : MqttNowBase() {};

void MqttNowBridge::begin() {
  if (!COM) {
    COM.begin(SERIALBAUDRATE);
  }

  PRINTLNS("Start listening on UART port");
}

void MqttNowBridge::update() {
  MqttNowBase::update();

  // Store UART input in a local buffer, until
  // CR (13) or LF (10) is received
  while (COM.available()) {
    char c = COM.read();
    if (c == 10 || c == 13) {
      if (_handleComm() == result_error) {
        PRINTLNS("Error handling command!");
        COM.println(RET_ERROR);
      } else {
        COM.println(RET_OK);
      }
      _comBuff = "";
    } else {
      _comBuff += c;
    }
  }
}

result_t MqttNowBridge::_handleComm() {
  PRINTLN("Communication received: ", _comBuff);
  #ifdef HAS_DISPLAY
  log2Display(("IN: "+_comBuff).c_str());
  #endif
  if (!_comBuff.startsWith(MSG_START)) {
    PRINTLNS("Unknown communication");
    return result_error;
  }

  // Get character after MSG_START (the action tag)
  char act = _comBuff.charAt(3);
  return _doAction(act);
}

result_t MqttNowBridge::_doAction(char act) {
  return result_error;
}
#endif