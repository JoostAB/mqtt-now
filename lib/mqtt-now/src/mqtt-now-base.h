/**
 * @file mqtt-now-base.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
# pragma once
#ifndef __MQTT_NOW_BASE_H__
#define __MQTT_NOW_BASE_H__

#include <baseinclude.h>

typedef int8_t result_t;
const result_t result_success = 1;
const result_t result_error = -1;

#define OTA_START "STARTOTA"
#define OTA_STOP "STOPOTA"

// Reference to the webserver, needed for OTA
extern WServer server;
extern bool stopWifiAfterOta;
extern bool serverRunning ;

class MqttNowBase {
  public:
    MqttNowBase();

    virtual void
      begin(),
      update();
  
  protected:
    void 
      startWifi(),
      stopWifi(),
      startServer(),
      stopServer(),
      startOTA(),
      stopOTA();

    uint8_t* _esp_now_id;
};

#endif // __MQTT_NOW_BASE_H__