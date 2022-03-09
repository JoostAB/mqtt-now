# pragma once
#if !defined(MQTT_NOW_CLIENT)
#ifndef __MQTT_NOW_NODE_H__
#define __MQTT_NOW_NODE_H__

#include <Arduino.h>
#include "mqtt-now-base.h"

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <espnow.h>
  
  // Following are defines from esp_now.h for ESP 32
  // Defined here for compatibility
  #define ESP_OK    0   /* Value indicating success (no error) */
  #define ESP_FAIL -1   /* Value indicating failure */
  
  typedef enum {
      ESP_NOW_SEND_SUCCESS = 0,       /* Send ESPNOW data successfully */
      ESP_NOW_SEND_FAIL,              /* Send ESPNOW data fail */
  } esp_now_send_status_t;
#elif defined(ESP32)
  #include <esp_now.h>
  #include <WiFi.h>

  esp_now_peer_info_t peerInfo;
#endif

/**
 * @brief A random key that will be used for encryption
 */
uint8_t _lmk[] = 
  {0x84, 0x0D, 0x8E, 0xAB, 
  0xB9, 0x37, 0x8E, 0xAB,
  0x0A, 0xC4, 0x97, 0x53,
  0xE6, 0x12, 0x00, 0xAB};

// Broadcast address to all peers
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData;

/*************************/
/** OnDataSent Callback **/
/*************************/
#ifdef ESP32
    void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  #elif defined(ESP8266)
    void OnDataSent(uint8_t *mac_addr, uint8_t status) {
  #endif
  PRINTS("Last Packet Send Status: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    PRINTLN("Delivery ", "success");
  } else {
    PRINTLN("Delivery ", "fail")
  }
  
}

/*****************************/
/** onDataReceiver Callback **/
/*****************************/
 #ifdef ESP32
    void onDataReceiver(const uint8_t * mac, const uint8_t *incomingData, int len) {
  #elif defined(ESP8266)
    void onDataReceiver(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  #endif
  PRINTLNS("Message received.");
  memcpy(&myData, incomingData, sizeof(myData));
  
  PRINTLN("Bytes received: ", len);
  PRINTLN("Char: ", myData.a);
  PRINTLN("Int: ", myData.b);
  PRINTLN("Float: ", myData.c);
  PRINTLN("Bool: ", myData.d);
  PRINTLF;
}
class MqttNowNode : public MqttNowBase {
  public:
    MqttNowNode();

    void
      begin(),
      update(); 
  
  protected:
    int addpeer(uint8_t *mac_addr, uint8_t channel, bool encrypt = false);
};
  
      
      

#endif // __MQTT_NOW_NODE_H__
#endif // !defined(MQTT_NOW_CLIENT)