# pragma once
#if !defined(MQTT_NOW_CLIENT)
#ifndef __MQTT_NOW_NODE_H__
#define __MQTT_NOW_NODE_H__

#include <Arduino.h>
#include <mqtt-now-base.h>

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
#endif



// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

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
    int addpeer(uint8_t *mac_addr, uint8_t channel, bool encrypt = false);
};

#endif // __MQTT_NOW_NODE_H__
#endif // !defined(MQTT_NOW_CLIENT)