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
// extern bool serverRunning ;

enum ComponentType {
  noneType,
  controllerType,
  alarmControlPanelType,
  binarySensorType,
  buttonType,
  cameraType,
  coverType,
  deviceTrackerType,
  deviceTriggerType,
  fanType,
  humidifierType,
  hvacType,
  lightType,
  lockType,
  numberType,
  sceneType,
  selectType,
  sensorType,
  sirenType,
  switchType,
  tagScannerType,
  vacuumType
};

const char* toName(ComponentType type);
/*
std::map<ComponentType, const char*> componentName = {
  {noneType, "none"},
  {controllerType, "controller"},
  {alarmControlPanelType, "alarmcontrolpanel"},
  {binarySensorType, "binarysensor"},
  {buttonType, "button"},
  {cameraType, "camera"},
  {coverType, "cover"},
  {deviceTrackerType, "devicetracker"},
  {deviceTriggerType, "devicetrigger"},
  {fanType, "fan"},
  {humidifierType, "humidifier"},
  {hvacType, "hvac"},
  {lightType, "light"},
  {lockType, "lock"},
  {numberType, "number"},
  {sceneType, "scene"},
  {selectType, "select"},
  {sensorType, "sensor"},
  {sirenType, "siren"},
  {switchType, "switch"},
  {tagScannerType, "tagscanner"},
  {vacuumType, "vacuum"}
};
*/
struct Node {
  String name;
  ComponentType component;
  String id;
} ;

class MqttNowBase {
  public:
    MqttNowBase();

    void
      setName(char* name),
      setType(ComponentType type),
      getMac(uint8_t *macaddr);

    ComponentType getType();
    
    String
      getName(),  
      getNodeId();

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

    Node  getNodeStruct();

  private:
    String _name;
    ComponentType _type = noneType;
    char _id[15];
    uint8_t* _esp_now_id;

};

#endif // __MQTT_NOW_BASE_H__