/**
 * @file mqtt-now-client.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief MQTT endpoint of the esp-now network.
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
# pragma once
#if defined(MQTT_NOW_CLIENT) | defined(MQTT_TEST_COMPILE)
#ifndef __MQTT_NOW_CLIENT_H__
#define __MQTT_NOW_CLIENT_H__

#include <baseinclude.h>
#include <mqtt-now-base.h>
#include <mqtt-now-bridge.h>
#include <PubSubClient.h>
#include <ESPConnect.h>
// #include <DNSServer.h>

// #ifdef ESP8266
// #include <ESP8266mDNS.h>
// #elif defined(ESP32)
// #include <ESPmDNS.h>
// #endif

#ifndef MQTT_PORT 
#define MQTT_PORT 1883
#endif

#ifndef MQTT_HOST
#error Need a host for MQTT broker. Please define MQTT_HOST before including mqtt-now.h
#define MQTT_HOST "none"
#endif

/**
 * @brief MQTT topic used for auto discovery
 * 
 */
#ifndef HA_DISCOVERY_TOPIC
#define HA_DISCOVERY_TOPIC "homeassistant"
#endif

#if !defined(COM)
#define COM Serial
#endif

// Callback
void mqttMsgReceived(char* topic, byte* payload, unsigned int length);

/**
 * @brief The MQTT client that connects to an MQTT broker over HTTP.
 * 
 * Is the bridge between the MQTT broker (over HTTP) and the MQTT-Now controller (over serial)
 * 
 */
class MqttNowClient : public MqttNowBase {
  public:
    /**
     * @brief Construct a new MqttNowClient object
     * 
     * @param host Hostname or IP address of the MQTT broker
     * @param mqttPort Port the MQTT broker is listening to
     * @param rootTopic MQTT root topic for MqttNow messages 
     * @param cmdTopic Device sub-topic for commands
     * @param statusTopic 
     * @param lwtTopic 
     * @param devTopic 
     * @param onCmd 
     * @param offCmd 
     * @param onlineLwt 
     * @param offlineLwt 
     */
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
      /**
       * @brief To be called from main setup
       * 
       */
      begin(),

      /**
       * @brief To be called from main loop
       * 
       */
      update(),
      setRootTopic(String rootTopic),
      setCmdTopic(String cmdTopic),
      setstatusTopic(String statusTopic),
      setLwtTopic(String lwtTopic),
      setDiscoveryTopic(String discoveryTopic),
      setDevicesTopic(String devTopic),
      setOnCmd(String onCmd),
      setOffCmd(String offCmd),
      setOnlineLwt(String onlineLwt),
      setOfflineLwt(String offlineLwt);
    
    result_t
      publishStatus(String status),
      publishCmd(String cmd),
      publish(String topic, String payload, bool retain = false, uint8_t qos = (uint8_t)1U),
      makeDiscoverable(),
      makeDiscoverable(Node node);
      

  private:
    void
      _setupWifi(),
      _callback(char* topic, byte* payload, unsigned int length),
      _reconnect();

    uint16_t _mqttPort = MQTT_PORT;
    uint32_t _timeout = 5000;
    result_t 
      _sendStringToController(const char* msg),
      _sendMqttMsgToController(),
      _handleComm(),
      _handleSubscribe(),
      _handleUnsubscribe(),
      _handleCommand(),
      _handleReboot(),
      _handlePublish(String com = String(""));

    String _modTopic(String topic),
           _getFullDiscoveryPath(Node node);
    
    String 
      _comBuff,
      _host,
      _rootTopic,
      _cmdTopic,
      _statusTopic,
      _lwtTopic,
      _devTopic,
      _discoveryTopic = "homeassistant",
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