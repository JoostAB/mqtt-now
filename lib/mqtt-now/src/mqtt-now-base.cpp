/**
 * @file mqtt-now-base.cpp
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <mqtt-now-base.h>

MqttNowBase::MqttNowBase() {
  // Open serial port for debugging if applicable
#if DEBUGLOG
  if (!Serial) { // But only if not already opened
   Serial.begin(SERIALBAUDRATE);
  }
  PRINTLNS("MqttNow object created");
#endif
};

void MqttNowBase::begin() {
  PRINTLNS("MqttNow started");
};
void MqttNowBase::update() {};