# pragma once
#ifndef __MQTT_NOW_NODE__
#define __MQTT_NOW_NODE__

#include <Arduino.h>
#include "mqtt-now-base.h"

class MqttNowNode : public MqttNowBase {
  public:
    MqttNowNode();

    void
      begin(),
      update(); 
};

#endif // __MQTT_NOW_NODE__