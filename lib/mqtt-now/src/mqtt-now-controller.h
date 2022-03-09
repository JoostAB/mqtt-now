# pragma once
#ifdef MQTT_NOW_CONTROLLER
#ifndef __MQTT_NOW_CONTROLLER_H__
#define __MQTT_NOW_CONTROLLER_H__

#include <Arduino.h>
#include <mqtt-now-node.h>

class MqttNowController : public MqttNowNode {
  public:
    MqttNowController();

    void
      begin(),
      update(); 
};

#endif // __MQTT_NOW_CONTROLLER_H__
#endif // MQTT_NOW_CONTROLLER