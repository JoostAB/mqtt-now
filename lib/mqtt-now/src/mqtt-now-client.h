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
#include <ArduinoJson.h>
#include <time.h>
#include <map>

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

#ifdef HASS_AUTODISCOVER
/**
 * @brief MQTT topic used for auto discovery
 * 
 */
#ifndef HA_DISCOVERY_TOPIC
#define HA_DISCOVERY_TOPIC "homeassistant"
#endif

enum ADCATEGORY {
  none,
  diagnostic,
  config
};



struct ADInfo {
  String component;
  String node;
  ADCATEGORY category;
  String stateTopic;
  String valtemplate;
  String name;
  String objectID;
  String uniqueID;
  bool availability;
  String availabilityTopic;
  String payloadAvailable;
  String payloadNotAvailable;
  String payloadOn;
  String payloadOff;
  String deviceClass;
  bool enabledDefault;
  String icon;
  String attrTopic;
  String attrTempl;
  /*
    device_class
    enabled_by_default
    icon
    "json_attributes_template": "{{ value_json.data.value | tojson }}",
    "json_attributes_topic": "zigbee2mqtt/bridge/response/networkmap",
  */
  ADInfo() : 
    category(none), 
    availability(true), 
    enabledDefault(false) {};
};
#endif

// Callback
void mqttMsgReceived(char* topic, byte* payload, unsigned int length);

/**
 * @brief The MQTT client that connects to an MQTT broker over HTTP.
 * 
 * Is the bridge between the MQTT broker (over HTTP) and the MQTT-Now controller (over serial)
 * 
 */
class MqttNowClient : public MqttNowBridge {
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
     * @param sysinfoTopic
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
      String sysinfoTopic = MQTT_SYSINFO_TOPIC,
      String onCmd = MQTT_ON_CMD, 
      String offCmd = MQTT_OFF_CMD, 
      String onlineLwt = MQTT_ONLINE_LWT,
      String offlineLwt = MQTT_OFFLINE_LWT);
  
    /** Webserver handlers **/
    /*
    static void handleUpdate();
    static void handleNotFound();
    static void handleCommand();
    */
    
      
#ifdef HASS_AUTODISCOVER
      result_t setupAutoDiscover();
      result_t setADstate();
      result_t setADaddress();
      result_t setADhostname();
      result_t setADboottime();
      result_t setADsysinfotime();
      result_t setADfreeheap();
      result_t setADcputemp();
      
#endif

    /**
     * @brief To be called from main setup
     * 
     */
    void begin();

    /**
     * @brief To be called from main loop
     * 
     */
    void update();
    void setCurrentTime();
    void setRootTopic(String rootTopic);
    void setCmdTopic(String cmdTopic);
    void setstatusTopic(String statusTopic);
    void setLwtTopic(String lwtTopic);
    void setDiscoveryTopic(String discoveryTopic);
    void setDevicesTopic(String devTopic);
    void setOnCmd(String onCmd);
    void setOffCmd(String offCmd);
    void setOnlineLwt(String onlineLwt);
    void setOfflineLwt(String offlineLwt);
    
    result_t
      publishStatus(String status),
      publishCmd(String cmd),
      publish(String topic, String payload, bool retain = false, uint8_t qos = (uint8_t)1U),
      publishSysInfo(),
      /**
       * @brief: Make a node discoverable by HA in MQTT (Not yet implemented)
       * 
       */
      makeDiscoverable(Node node),
      _doAction(char act);
      
    
  private:
    String
      _lastPublishedTopic,
      _lastPublishedPayload,
      _bootTime;

    void
      _setupWifi(),
      _callback(char* topic, byte* payload, unsigned int length),
      _reconnect();

    JsonObject* getObject(JsonDocument* doc);
    
    String _timeStructToString(tm* time);
    
    uint16_t _mqttPort = MQTT_PORT;
    uint32_t _timeout = 5000;
    result_t 
      _sendStringToController(const char* msg),
      _sendMqttMsgToController(),
      //_handleComm(),
      _handleSubscribe(),
      _handleUnsubscribe(),
      _handleCommand(),
      _handleReboot(),
      _handlePublish(String com = String(""));

    String _modTopic(String topic),
           _getFullDiscoveryPath(Node node),
           _getCurrentTime();
    
    
    String 
      _host,
      _rootTopic,
      _cmdTopic,
      _statusTopic,
      _lwtTopic,
      _devTopic,
      _sysinfoTopic,
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

    #ifdef HASS_AUTODISCOVER
    result_t _postADinfo(ADInfo* info);
    #endif
};

#endif // __MQTT_NOW_CLIENT_H__
#endif // MQTT_NOW_CLIENT