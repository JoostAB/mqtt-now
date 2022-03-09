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
};

#endif // __MQTT_NOW_NODE_H__
#endif // !defined(MQTT_NOW_CLIENT)