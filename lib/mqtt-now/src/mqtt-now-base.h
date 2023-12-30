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



#define CMD_OTA_START     "STARTOTA"
#define CMD_OTA_STOP      "STOPOTA"
#define CMD_CTRL_REBOOT   "REBOOTCTRL"
#define CMD_CLIENT_REBOOT "REBOOTCLIENT"
#define CMD_NODE_REBOOT   "REBOOT"

// Reference to the webserver, needed for OTA
extern WServer server;
extern bool stopWifiAfterOta; 
// extern bool serverRunning ;

/**
 * @brief Enumeration of possible component types.
 * 
 * Can be used by a clientapplication like HomeAssistant to define some basic
 * properties like capabilities or icon.
 * 
 */
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

/**
 * @brief Base class for all MqttNow classes
 * 
 * Defines basic functionalities.
 * 
 */
class MqttNowBase {
  public:
    /**
     * @brief Construct a new Mqtt Now Base object
     * 
     */
    MqttNowBase();

    void
      /**
       * @brief Set the name for this node
       * 
       * @param name 
       */
      setName(char* name),

      /**
       * @brief Set the type of component
       * 
       * @param type One of ComponentType
       */
      setType(ComponentType type),

      /**
       * @brief Get the Mac Address of this node
       * 
       * @param macaddr 
       */
      getMac(uint8_t *macaddr);

    ComponentType getType();
    
    String
      /**
       * @brief Returns the name
       * 
       */
      getName(),  

      /**
       * @brief Generates and returns an identifier for this node
       * 
       * For now this is the formatted MAC address as String
       * 
       */
      getNodeId();

    virtual void
      /**
       * @brief Initializes the node
       * 
       * To be called once from the main setup.
       */
      begin(),

      /**
       * @brief Running loop of the node
       * 
       * To be called as often as possible from main loop
       */
      update();
  
  protected:
    void 

      /**
       * @brief Start WiFi radio and connect to SSID
       * 
       */
      startWifi(),

      /**
       * @brief Stop the wifi radio
       * 
       */
      stopWifi(),

      /**
       * @brief Start the webserver
       * 
       */
      startServer(),

      /**
       * @brief Stop the webserver
       * 
       */
      stopServer(),

      /**
       * @brief Start listening to OTA packages
       * 
       */
      startOTA(),

      /**
       * @brief Stop listening to OTA packages
       * 
       */
      stopOTA();

    /**
     * @brief Generate and return a Node structure
     * 
     * @return Node 
     */
    Node  getNodeStruct();

  private:
    String _name;
    ComponentType _type = noneType;
    char _id[15];
    uint8_t* _esp_now_id;

};

#endif // __MQTT_NOW_BASE_H__