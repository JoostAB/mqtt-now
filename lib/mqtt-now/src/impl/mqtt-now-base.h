# pragma once
#ifndef __MQTT_NOW_BASE__
#define __MQTT_NOW_BASE__

#include <Arduino.h>

class MqttNowBase {
  public:
    MqttNowBase();

    virtual void
      begin(),
      update();
  
  protected:
    uint8_t* _esp_now_id;
};

#endif // __MQTT_NOW_BASE__