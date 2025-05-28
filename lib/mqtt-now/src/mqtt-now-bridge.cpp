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
  MqttNowBase::begin();
  initUart();

  PRINTLNS("Start listening on UART port");
}

void MqttNowBridge::update() {
  MqttNowBase::update();

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
        _comBuff.clear();
        _comBuff = buff;
      }
      #endif
      if (_handleComm() == result_error) {
        PRINTLNS("Error handling command!");
        uart.println(RET_ERROR);
      } else {
        uart.println(RET_OK);
      }
      buff.clear();
    } else {
      buff += c;
    }
  }
}

result_t MqttNowBridge::initUart() {
  if (!COM) {
    if (Serial != COM) {
      //COM.begin(SERIALBAUDRATE, SERIAL_8N1, /* RX */ 1, /* TX */ 0);
      PRINTF2("Initializing UART port. RX pin = %i, TX pin = %i", RX_PIN, TX_PIN);
      PRINTLF
      COM.begin(SERIALBAUDRATE, SERIAL_8N1, /* RX */ RX_PIN, /* TX */ TX_PIN);
    } else {
      COM.begin(SERIALBAUDRATE);
    }
    yield();
  }
  return result_success;
}

result_t MqttNowBridge::sendSerial(const char* msg) {
  if (!String(msg).startsWith(MSG_START)) {
    PRINTLNS("No valid message to send");
    return result_error;
  }
  initUart();
  PRINTS("Sending over uart: ");
  PRINTLNSA(msg);
  #ifdef HAS_DISPLAY
  log2Display("OUT:");
  log2Display(msg);
  #endif
  size_t send = 0;
  send += COM.print(msg);
  COM.println();
  
  PRINTLN("Nr of bytes send over uart: ", send);
  return (send > 0)?result_success:result_error;
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