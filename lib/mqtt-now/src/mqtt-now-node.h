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

typedef uint8_t msgType;

typedef struct msg_base {
  msgType msgtype;
  msgType msgtype;
} msg_base;

typedef struct msg_intro:msg_base {
  uint8_t mac_address[6];
  uint8_t network_uuid[16];
  uint8_t device_category;
  uint16_t timeout;
  char friendly_name[30];
} msg_intro;

typedef struct msg_welcome:msg_base {
  uint8_t mac_address[6];
} msg_welcome;

typedef struct msg_reqconfig:msg_base {
  // Empty structure.
} msg_reqconfig;

typedef struct msg_config:msg_base {
  char wifi_ssid[32];
  char wifi_key[64];
  uint8_t mqtt_ip[4];
  uint16_t mqtt_port;
  char mqtt_user[20];
  char mqtt_pw[20];
} msg_config;

typedef struct msg_data:msg_base {
  uint8_t data_type;
  char data[240];
} msg_data;

typedef struct msg_error:msg_base {
  uint8_t error_code;
  char error_msg[240];
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
  void OnDataSent(uint8_t *mac_addr, uint8_t status);
  void onDataReceiver(uint8_t * mac, uint8_t *incomingData, uint8_t len);
#endif

class MqttNowNode : public MqttNowBase {
  public:
    MqttNowNode();

    void
      /**
       * @brief To be called from main setup
       * 
       */
      begin(),

      /**
       * @brief To be called as often as possible from main loop
       * 
       */
      update();

    virtual void 
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
      messageReceived(const uint8_t *macFrom, msgType type, msg_base *msg, uint8_t len) = 0;
    
  
  protected:
    esp_err_t 
      addPeer(uint8_t *mac_addr, uint8_t channel, bool encrypt = false),
      addPeer(esp_now_peer_info_t *peer),
      sendMessage(msgType type, msg_base *msg, const uint8_t *macReceive),
      sendMessage(msg_base *msg, const uint8_t *macReceive),
      sendIntroMessage(uint8_t category, char friendlyName[30], const uint8_t *macReceiver),
      sendWelcomeMessage(const uint8_t *macReceiver),
      sendReqCfgMessage(const uint8_t *macReceiver),
      sendErrorMessage(uint8_t error_code, const char error_msg[240], const uint8_t *macReceiver);


    
    result_t getMessageStruct(msgType type, msg_base *msg);
  
};

#endif // __MQTT_NOW_NODE_H__
#endif // !defined(MQTT_NOW_CLIENT)