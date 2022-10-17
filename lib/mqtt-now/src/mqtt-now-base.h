# pragma once
#ifndef __MQTT_NOW_BASE_H__
#define __MQTT_NOW_BASE_H__

#include <Arduino.h>
#include <jbdebug.h>

typedef bool mqttnow_success;

/*
Supported components by by HA MQTT discovery:
Alarm control panels
Binary sensors
Buttons
Cameras
Covers
Device Trackers
Device Triggers
Fans
Humidifiers
HVACs
Lights
Locks
Numbers
Scenes
Selects
Sensors
Sirens
Switches
Tag Scanners
Vacuums
*/

enum ComponentType {
  alarmControlPanel,
  binarySensor,
  button,
  camera,
  cover,
  deviceTracker,
  deviceTrigger,
  fan,
  humidifier,
  hvac,
  light,
  lock,
  number,
  scene,
  select,
  sensor,
  siren,
  switche,
  tagScanner,
  vacuum
};

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