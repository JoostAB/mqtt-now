
#if !(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))
#ifndef __MQTT_NOW_SLAVE_H__
#define __MQTT_NOW_SLAVE_H__

#include <Arduino.h>
#include "mqtt-now-node.h"

class MqttNowSlave : public MqttNowNode {
  public:
    MqttNowSlave();

    void
      begin(),
      update(); 
};

#endif // __MQTT_NOW_SLAVE_H__
#endif // !(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))