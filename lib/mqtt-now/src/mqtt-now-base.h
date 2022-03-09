# pragma once
#ifndef __MQTT_NOW_BASE_H__
#define __MQTT_NOW_BASE_H__

#include <baseinclude.h>

typedef bool mqttnow_success;

class MqttNowBase {
  public:
    MqttNowBase();

    virtual void
      begin(),
      update();
  
  protected:
    uint8_t* _esp_now_id;
};

#endif // __MQTT_NOW_BASE_H__