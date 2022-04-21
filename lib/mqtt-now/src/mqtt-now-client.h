/**
 * @file mqtt-now-client.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
# pragma once
#ifdef MQTT_NOW_CLIENT
#ifndef __MQTT_NOW_CLIENT_H__
#define __MQTT_NOW_CLIENT_H__

#include <baseinclude.h>
#include <mqtt-now-base.h>
#include <mqtt-now-bridge.h>
#include <PubSubClient.h>

#include <DNSServer.h>
#include <ESP8266mDNS.h>

#ifndef MQTT_PORT 
#define MQTT_PORT 1883
#endif

#ifndef MQTT_HOST
// TODO: uncomment for release
// #error Need a host for MQTT broker. Please define MQTT_HOST before including mqtt-now.h
#define MQTT_HOST "none"
#endif

#if !defined(COM)
#define COM Serial
#endif

// Callback
void mqttMsgReceived(char* topic, byte* payload, unsigned int length);

class MqttNowClient : public MqttNowBase {
  public:
    MqttNowClient(
      String host = MQTT_HOST, 
      uint16_t mqttPort = MQTT_PORT,
      String rootTopic = MQTT_ROOT_TOPIC, 
      String cmdTopic = MQTT_CMD_TOPIC, 
      String statusTopic = MQTT_STATUS_TOPIC, 
      String lwtTopic = MQTT_LWT_TOPIC, 
      String devTopic = MQTT_DEV_TOPIC, 
      String onCmd = MQTT_ON_CMD, 
      String offCmd = MQTT_OFF_CMD, 
      String onlineLwt = MQTT_ONLINE_LWT,
      String offlineLwt = MQTT_OFFLINE_LWT);
  
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
      setDevicesTopic(String devTopic),
      setOnCmd(String onCmd),
      setOffCmd(String offCmd),
      setOnlineLwt(String onlineLwt),
      setOfflineLwt(String offlineLwt);
    
    result_t
      publishStatus(String status),
      publishCmd(String cmd),
      publish(String topic, String payload, bool retain = false, uint8_t qos = (uint8_t)1U);
      

  private:
    void
      _setupWifi(),
      _callback(char* topic, byte* payload, unsigned int length),
      _reconnect();

    uint16_t _mqttPort = MQTT_PORT;
    uint32_t _timeout = 5000;
    result_t 
      _sendMsgToController(),
      _handleComm(),
      _handleSubscribe(),
      _handleUnsubscribe(),
      _handleCommand(),
      _handlePublish(String com = String(""));

    String _modTopic(String topic);

    String 
      _comBuff,
      _host,
      _rootTopic,
      _cmdTopic,
      _statusTopic,
      _lwtTopic,
      _devTopic,
      _onCmd,
      _offCmd,
      _onlineLwt,
      _offlineLwt,
      _mqttId,
      _mqttUser,
      _mqttPwd;
    WiFiClient* _wifiClient;
    PubSubClient* _mqttClient;
};

#endif // __MQTT_NOW_CLIENT_H__
#endif // MQTT_NOW_CLIENT