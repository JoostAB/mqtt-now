# pragma once
#ifndef __MQTT_NOW__
#define __MQTT_NOW__

#include <Arduino.h>


#ifdef MQTT_NOW_CLIENT
  #include "impl/mqtt-now-client.h"
  typedef MqttNowClient MqttNow;
#elif defined(MQTT_NOW_CONTROLLER)
  #include "impl/mqtt-now-controller.h"
  typedef MqttNowController MqttNow;
#else
  //#include <mqtt-now-node.h>
  #include "impl/mqtt-now-node.h"
  typedef MqttNowNode MqttNow;
#endif





#endif // __MQTT_NOW__