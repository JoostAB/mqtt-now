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