/**
 * @file mqtt-now-controller.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#if defined(MQTT_NOW_CONTROLLER) | defined(MQTT_TEST_COMPILE)
#ifndef __MQTT_NOW_CONTROLLER_H__
#define __MQTT_NOW_CONTROLLER_H__

#include <baseinclude.h>
#include <mqtt-now-node.h>
#include <mqtt-now-bridge.h>
#include <list>

#if !defined(COM)
#define COM Serial
#endif

using namespace std;

class MqttNowController : public MqttNowNode {
  public:
    MqttNowController();

    void
      messageReceived(const uint8_t *macFrom, uint8_t type, msg_base *msg, uint8_t len),
      begin(),
      update(); 
  
  private:
    std::list<esp_now_peer_info_t> _slaves;
};

#endif // __MQTT_NOW_CONTROLLER_H__
#endif // MQTT_NOW_CONTROLLER