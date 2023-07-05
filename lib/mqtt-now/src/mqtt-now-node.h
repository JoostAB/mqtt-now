/**
 * @file mqtt-now-node.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
# pragma once
#if (!defined(MQTT_NOW_CLIENT)) | defined(MQTT_TEST_COMPILE)
#ifndef __MQTT_NOW_NODE_H__
#define __MQTT_NOW_NODE_H__

#include <baseinclude.h>
#include <mqtt-now-base.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <espnow.h>
  #include <esp8266-now-mock.h>

#elif defined(ESP32)
  #include <esp_now.h>
  #include <WiFi.h>
#endif

typedef uint8_t msgType;

#define SIZE_SSID 32
#define SIZE_WIFIKEY 64
#define SIZE_USERNAME 20
#define SIZE_PW 20
#define SIZE_UUID 16
#define SIZE_FRIENDNAME 30
#define SIZE_DATA 240
#define SIZE_ERRMSG 240

/**
 * @brief Base definition of a message struct
 */
typedef struct msg_base {
  /**
   * @brief The type of this message. One of MsgType
   */
  msgType msgtype;
} msg_base;

/**
 * @brief Intro message.
 * 
 * Message used by  node to introduce itself to the network
 */
typedef struct msg_intro:msg_base {
  /**
   * @brief MAC Address of sending node
   */
  uint8_t mac_address[SIZE_MAC];

  /**
   * @brief Unique (secret) 16 byte ID of the network
   * 
   * Only if this ID equals the ID of the network the node is added. Otherwise it is ignored
   * and, if capable, the node will start its own network.
   */
  uint8_t network_uuid[SIZE_UUID];

  /**
   * @brief Category of the device.
   * 
   * One of ComponentType
   */
  uint8_t device_category;

  /**
   * @brief Amount of minutes of silence after which the controller should mark this node as 'dead'
   */
  uint16_t timeout;

  /**
   * @brief Friendly human readable name
   * 
   * Can be used in interfaces to clearly describe a node.
   */
  char friendly_name[SIZE_FRIENDNAME];
} msg_intro;

typedef struct msg_welcome:msg_base {
  uint8_t mac_address[SIZE_MAC];
} msg_welcome;

typedef struct msg_reqconfig:msg_base {
  // Empty structure.
} msg_reqconfig;

typedef struct msg_config:msg_base {
  char wifi_ssid[SIZE_SSID];
  char wifi_key[SIZE_WIFIKEY];
  uint8_t mqtt_ip[SIZE_IP];
  uint16_t mqtt_port;
  char mqtt_user[SIZE_USERNAME];
  char mqtt_pw[SIZE_PW];
} msg_config;

typedef struct msg_data:msg_base {
  uint8_t data_type;
  int8_t data[SIZE_DATA];
} msg_data;

typedef struct msg_error:msg_base {
  uint8_t error_code;
  char error_msg[SIZE_ERRMSG];
} msg_error;

/**
 * @brief Message Wrapper structure. 
 * 
 * The actual message that is being send and contains the message
 * type and the message structure.
 * Type can be one of:
 * - 1: Intro message
 * - 2: Welcome message
 * - 3: Request Config message
 * - 4: Config message
 * - 5: Data message
 * - 6: Error message
 * 
 */
typedef struct struct_msg {
  msgType type;
  uint8_t msgSize;
  msg_base* contents;
} struct_msg;

const msgType msgTypeIntro     = 1;
const msgType msgTypeWelcome   = 2;
const msgType msgTypeReqConfig = 3;
const msgType msgTypeConfig    = 4;
const msgType msgTypeData      = 5;
const msgType msgTypeError     = 6;

/*************************
 *  Callback prototypes  * 
 *************************/
#if defined(ESP32)
  /**
   * @brief Is called whenever esp-now data is sent
   * 
   * @param mac_addr Mac address of the receiver
   * @param status One of esp_now_send_status_t
   */
  void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

  /**
   * @brief Is called whenever esp-now data is received
   * 
   * @param mac MAC address of the sender
   * @param incomingData Array of bytes containing the incoming data
   * @param len Nr of bytes of data
   */
  void onDataReceiver(const uint8_t * mac, const uint8_t *incomingData, int len);
#elif defined(ESP8266)

  /**
   * @brief Is called whenever esp-now data is sent
   * 
   * @param mac_addr Mac address of the receiver
   * @param status One of esp_now_send_status_t
   */
  void OnDataSent(uint8_t *mac_addr, uint8_t status);
  
  /**
   * @brief Is called whenever esp-now data is received
   * 
   * @param mac MAC address of the sender
   * @param incomingData Array of bytes containing the incoming data
   * @param len Nr of bytes of data
   */
  void onDataReceiver(uint8_t * mac, uint8_t *incomingData, uint8_t len);
#endif

class MqttNowNode : public MqttNowBase {
  public:
    MqttNowNode();

    /**
     * @brief To be called from main setup
     * 
     */
    void begin();

    /**
     * @brief To be called as often as possible from main loop
     * 
     */
    void update();

    /**
     * @brief Is triggered whenever a message is received over the esp-now network from another MqttNowNode.
     * 
     * To be implemented in concrete sub-classes
     * 
     * @param macFrom Origin MAC address
     * @param type Type of message (one of msgType)
     * @param msg 
     * @param len 
     */
    virtual void messageReceived(const uint8_t *macFrom, msgType type, msg_base *msg, uint8_t len) = 0;
    
  
  protected:
    esp_err_t addPeer(uint8_t *mac_addr, uint8_t channel, bool encrypt = false);
    esp_err_t addPeer(esp_now_peer_info_t *peer);
    esp_err_t sendMessage(msgType type, msg_base *msg, const uint8_t *macReceive);
    esp_err_t sendMessage(msg_base *msg, const uint8_t *macReceive);
    esp_err_t sendIntroMessage(uint8_t category, char friendlyName[SIZE_FRIENDNAME], const uint8_t *macReceiver);
    esp_err_t sendWelcomeMessage(const uint8_t *macReceiver);
    esp_err_t sendReqCfgMessage(const uint8_t *macReceiver);
    esp_err_t sendCfgMessage(const char *wifi_ssid, const char *wifi_key, const uint8_t mqtt_ip[SIZE_IP], 
                     const uint16_t mqtt_port, const char *mqtt_user, const char *mqtt_pw, const uint8_t *macReceiver);
    esp_err_t sendDataMessage(uint8_t data_type, const int8_t *data, const uint8_t *macReceiver);
    esp_err_t sendErrorMessage(uint8_t error_code, const char error_msg[SIZE_ERRMSG], const uint8_t *macReceiver);

    result_t getMessageStruct(msgType type, msg_base *msg);
  
};

#endif // __MQTT_NOW_NODE_H__
#endif // !defined(MQTT_NOW_CLIENT)