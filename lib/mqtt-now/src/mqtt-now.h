/**
 * @file mqtt-now.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
# pragma once
#ifndef __MQTT_NOW_H__
#define __MQTT_NOW_H__

#include <baseinclude.h>

#ifdef MQTT_NOW_CLIENT
  #include <mqtt-now-client.h>
  typedef MqttNowClient MqttNow;
#elif defined(MQTT_NOW_CONTROLLER)
  #include <mqtt-now-controller.h>
  typedef MqttNowController MqttNow;
#else
  #include <mqtt-now-slave.h>
  typedef MqttNowSlave MqttNow;
#endif

#endif // __MQTT_NOW_H__