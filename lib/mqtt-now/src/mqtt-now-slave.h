/**
 * @file mqtt-now-slave.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#if (!(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))) | defined(MQTT_TEST_COMPILE)
#ifndef __MQTT_NOW_SLAVE_H__
#define __MQTT_NOW_SLAVE_H__

#include <baseinclude.h>
#include <mqtt-now-node.h>

class MqttNowSlave : public MqttNowNode {
  public:
    MqttNowSlave();

    void
      messageReceived(const uint8_t *macFrom, uint8_t type, msg_base *msg, uint8_t len),
      begin(),
      update(); 
};

#endif // __MQTT_NOW_SLAVE_H__
#endif // !(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))