# pragma once
#ifdef MQTT_NOW_CLIENT
#ifndef __MQTT_NOW_CLIENT_H__
#define __MQTT_NOW_CLIENT_H__

#include <Arduino.h>
#include "mqtt-now-base.h"

// #ifdef ESP8266
//   #include <ESP8266WiFi.h>
// #elif defined(ESP32)
//   #include <WiFi.h>
// #endif

#define AP_NAME "MQTTNow bridge"
#define AP_PASSWORD "MQTTNow"
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

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

void handleUpdate();
void handleNotFound();
void handleCommand();

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
      // handleUpdate(),
      // handleNotFound(),
      // handleCommand();

  private:
    
};

#endif // __MQTT_NOW_CLIENT_H__
#endif // MQTT_NOW_CLIENT