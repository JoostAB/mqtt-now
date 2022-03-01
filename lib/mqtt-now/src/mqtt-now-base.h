# pragma once
#ifndef __MQTT_NOW_BASE_H__
#define __MQTT_NOW_BASE_H__

#include <Arduino.h>

// #ifdef ESP32
// #else
//   #include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//   //needed for library
//   #include <ESP8266WebServer.h>
// #endif

// #include <DNSServer.h>

class MqttNowBase {
  public:
    MqttNowBase();

    virtual void
      begin(),
      update();
  
  protected:
    uint8_t* _esp_now_id;
};

#endif // __MQTT_NOW_BASE_H__