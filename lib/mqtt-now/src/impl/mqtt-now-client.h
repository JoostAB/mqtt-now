# pragma once
#ifndef __MQTT_NOW_CLIENT__
#define __MQTT_NOW_CLIENT__

#include <Arduino.h>
#include "mqtt-now-base.h"

#ifdef ESP8266
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif

#ifndef MQTT_ROOT_TOPIC
#define MQTT_ROOT_TOPIC "mqtt-now"
#endif

#ifndef MQTT_CMD_TOPIC
#define MQTT_CMD_TOPIC "cmd"
#endif

#ifndef MQTT_STATUS_TOPIC
#define MQTT_STATUS_TOPIC "status"
#endif

#ifndef MQTT_LWT_TOPIC
#define MQTT_LWT_TOPIC "lwt"
#endif

#ifndef MQTT_ON_CMD
#define MQTT_ON_CMD "on"
#endif

#ifndef MQTT_OFF_CMD
#define MQTT_OFF_CMD "off"
#endif

#ifndef MQTT_ONLINE_LWT
#define MQTT_ONLINE_LWT "online"
#endif

#ifndef MQTT_OFFLINE_LWT
#define MQTT_OFFLINE_LWT "offline"
#endif

#ifndef MQTT_PORT 
#define MQTT_PORT 1883
#endif

#ifndef MQTT_HOST
//#error Need a host for MQTT broker. Please define MQTT_HOST before including mqtt-now.h
#endif

class MqttNowClient : public MqttNowBase {
  public:
    // MqttNowClient(String host, uint16_t port = 1883);
    // MqttNowClient(String host, String rootTopic, uint16_t port = 1883);
    // MqttNowClient(String host, String rootTopic, String cmdTopic = "cmd", String statusTopic = "status", 
    //     String lwtTopic = "lwt", String onCmd = "ON", String offCmd = "OFF", String onlineLwt = "Online",
    //     String offlineLwt = "Offline", uint16_t port = 1883);
  
    void
      begin(),
      update();
};

#endif // __MQTT_NOW_CLIENT__