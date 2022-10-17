# pragma once
#ifdef MQTT_NOW_CLIENT
#ifndef __MQTT_NOW_CLIENT_H__
#define __MQTT_NOW_CLIENT_H__

#include <Arduino.h>
#include "mqtt-now-base.h"
#include <MQTTPubSubClient.h>

// #ifdef ESP8266
//   #include <ESP8266WiFi.h>
// #elif defined(ESP32)
//   #include <WiFi.h>
// #endif

#define AP_NAME "MQTTNow bridge"
// #define AP_PASSWORD "MQTTNow"
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
// TODO: uncomment for release
//#error Need a host for MQTT broker. Please define MQTT_HOST before including mqtt-now.h
#define MQTT_HOST "none"
#endif

#define HA_DISCOVERY_TOPIC "homeassistant"

class MqttNowClient : public MqttNowBase {
  public:
    MqttNowClient();
    MqttNowClient(String host, uint16_t port = MQTT_PORT);
    MqttNowClient(String host, String rootTopic, uint16_t port = MQTT_PORT);
    MqttNowClient(
      String host, 
      String rootTopic, 
      String cmdTopic = MQTT_CMD_TOPIC, 
      String statusTopic = MQTT_STATUS_TOPIC, 
      String lwtTopic = MQTT_LWT_TOPIC, 
      String onCmd = MQTT_ON_CMD, 
      String offCmd = MQTT_OFF_CMD, 
      String onlineLwt = MQTT_ONLINE_LWT,
      String offlineLwt = MQTT_OFFLINE_LWT, 
      uint16_t mqttPort = MQTT_PORT);
  
    static void
      handleUpdate(),
      handleNotFound(),
      handleCommand();

    void
      begin(),
      update(),
      setRootTopic(String rootTopic),
      setCmdTopic(String cmdTopic),
      setstatusTopic(String statusTopic),
      setLwtTopic(String lwtTopic),
      setDiscoveryTopic(String discoveryTopic),
      setOnCmd(String onCmd),
      setOffCmd(String offCmd),
      setOnlineLwt(String onlineLwt),
      setOfflineLwt(String offlineLwt);
    
    mqttnow_success
      start(),
      start(WiFiClient* wifiClient),
      publishStatus(String status),
      publishCmd(String cmd),
      publish(String topic, String payload, bool retain = false, uint8_t qos = (uint8_t)0U),
      makeDiscoverable();
      

  private:
    void
      _startWebserver(),
      _startMqttClient();

    uint16_t _mqttPort = MQTT_PORT;
    uint32_t _timeout = 5000;
    String 
          _host,
          _rootTopic,
          _cmdTopic,
          _statusTopic,
          _lwtTopic,
          _discoveryTopic,
          _onCmd,
          _offCmd,
          _onlineLwt,
          _offlineLwt,
          _mqttId,
          _mqttUser,
          _mqttPwd;
    WiFiClient* _wifiClient;
    MQTTPubSubClient* _mqttClient;
};

#endif // __MQTT_NOW_CLIENT_H__
#endif // MQTT_NOW_CLIENT