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

MqttNowBridge::MqttNowBridge() : BRIDGEPARENT() {};

void MqttNowBridge::begin() {
  if (!COM) {
    COM.begin(SERIALBAUDRATE);
  }

  PRINTLNS("Start listening on UART port");
}

void MqttNowBridge::update() {
  BRIDGEPARENT::update();

  _readSerial(COM, _comBuff);
  #ifdef DEBUGLOG
    _readSerial(Serial, _serBuff);
  #endif
}

void MqttNowBridge::_readSerial(Stream& uart, String& buff) {
  // Store UART input in a local buffer, until
  // CR (13) or LF (10) is received
  while (uart.available()) {
    char c = uart.read();
    if (c == 10 || c == 13) {
      #ifdef DEBUGLOG
      // If DEBUGLOG is enabled, check if passed Stream is the
      // debug serial connection. If so, copy buffer to _commBuff
      // so that the command can be handled
      if (&uart != &COM) {
        _comBuff = buff;
      }
      #endif
      if (_handleComm() == result_error) {
        PRINTLNS("Error handling command!");
        uart.println(RET_ERROR);
      } else {
        uart.println(RET_OK);
      }
      buff = "";
    } else {
      buff += c;
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
  // To be implemented in descendant
  return result_error;
}

/** Utilities **/
String MqttNowBridge::_modTopic(String topic) {
  String ret;
  switch (topic.charAt(0)) {
    case '/':
      ret = _devTopic + PATHSEP + topic.substring(1);
      break;
    case '@':
      ret = _rootTopic + PATHSEP + topic.substring(1);
      break;
    default:
      ret = topic;
      break;
  }
  return ret;
}
#endif