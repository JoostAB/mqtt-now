# pragma once
#ifdef MQTT_NOW_CONTROLLER
#ifndef __MQTT_NOW_CONTROLLER_H__
#define __MQTT_NOW_CONTROLLER_H__

#include <baseinclude.h>
#include <mqtt-now-node.h>
#include <list>
using namespace std;

class MqttNowController : public MqttNowNode {
  public:
    MqttNowController();

    void
      begin(),
      update(); 
  
  private:
    std::list<esp_now_peer_info_t> _slaves;
};

#endif // __MQTT_NOW_CONTROLLER_H__
#endif // MQTT_NOW_CONTROLLER