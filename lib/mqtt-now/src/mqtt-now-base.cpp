#include <mqtt-now-base.h>

MqttNowBase::MqttNowBase() {
  // Open serial port for debugging if applicable
#if DEBUGLOG
  if (!Serial) { // But only if not already opened
   Serial.begin(SERIALBAUDRATE);
  }
#endif
};

void MqttNowBase::begin() {};
void MqttNowBase::update() {};