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

/**
 * @brief Base for the end-device.
 * 
 * Use this as a base class for the implementation of sensors, actuators, lights etc.
 */
class MqttNowSlave : public MqttNowNode {
  public:
    /**
     * @brief  Constructs a new MQTT-Now slave
     */
    MqttNowSlave();

    void
    /**
       * @brief Is triggered whenever a message is received over the esp-now network from an MqttNowNode.
       * 
       * @param macFrom Origin MAC address
       * @param type Type of message (one of msgType)
       * @param msg 
       * @param len 
       */
      messageReceived(const uint8_t *macFrom, uint8_t type, msg_base *msg, uint8_t len),

      /**
       * @brief To be called once at startup. 
       */
      begin(),

      /**
       * @brief To be called as often as possible from main loop
       */
      update(); 
};

#endif // __MQTT_NOW_SLAVE_H__
#endif // !(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))