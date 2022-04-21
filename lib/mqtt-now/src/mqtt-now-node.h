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
#if !defined(MQTT_NOW_CLIENT)
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

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;


typedef struct msg_base {} msg_base;

typedef struct msg_intro:msg_base {
  uint8_t mac_address[6];
  uint8_t network_uuid[16];
  uint8_t device_category;
  char firendly_name[30];
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

/**
 * Wrapper structure. 
 * The actual message that is being send and contains the message
 * type and the message structure.
 * Type can be one of:
 * - 1: Intro message
 * - 2: Welcome message
 * - 3: Request Config message
 * - 4: Config message
 * - 5: Data message
 **/
typedef struct struct_msg {
  uint8_t msgType;
  uint8_t msgSize;
  msg_base *contents;
} struct_msg;

/*************************/
/** Callback prototypes **/
/*************************/
#if defined(ESP32)
  void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
  void onDataReceiver(const uint8_t * mac, const uint8_t *incomingData, int len);
#elif defined(ESP8266)
  void OnDataSent(uint8_t *mac_addr, uint8_t status);
  void onDataReceiver(uint8_t * mac, uint8_t *incomingData, uint8_t len);
#endif

class MqttNowNode : public MqttNowBase {
  public:
    MqttNowNode();

    void
      begin(),
      update(); 
  
  protected:
    esp_err_t addPeer(uint8_t *mac_addr, uint8_t channel, bool encrypt = false);
    esp_err_t addPeer(esp_now_peer_info_t *peer);
    esp_err_t sendMessage(uint8_t type, msg_base *msg, uint8_t *macReceive);
};

#endif // __MQTT_NOW_NODE_H__
#endif // !defined(MQTT_NOW_CLIENT)