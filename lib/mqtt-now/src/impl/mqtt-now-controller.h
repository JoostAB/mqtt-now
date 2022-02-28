# pragma once
#ifndef __MQTT_NOW_CONTROLLER__
#define __MQTT_NOW_CONTROLLER__

#include <Arduino.h>
#include "mqtt-now-base.h"

class MqttNowController : public MqttNowBase {
  public:
    MqttNowController();

    void
      begin(),
      update(); 
};

#endif // __MQTT_NOW_CONTROLLER__