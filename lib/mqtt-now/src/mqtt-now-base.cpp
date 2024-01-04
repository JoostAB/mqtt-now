/**
 * @file mqtt-now-base.cpp
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <mqtt-now-base.h>
// #include <AsyncElegantOTA.h>

WServer server(80);
bool stopWifiAfterOta = true;
bool serverRunning = false;

#ifdef ESP32
  #include <esp_wifi.h>
  void getmac(uint8_t *macaddr) {
    if(WiFiGenericClass::getMode() == WIFI_MODE_NULL){
        esp_read_mac(macaddr, ESP_MAC_WIFI_STA);
    }
    else{
        esp_wifi_get_mac((wifi_interface_t)ESP_IF_WIFI_STA, macaddr);
    }
  }
#endif
#ifdef ESP8266
  void getmac(uint8_t *macaddr) {wifi_get_macaddr(STATION_IF, macaddr);}
#endif

const char* toName(ComponentType type) {
  return "";
}
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

MqttNowBase::MqttNowBase() {
  // Open serial port for debugging if applicable
#if DEBUGLOG
  if (!Serial) { // But only if not already opened
   Serial.begin(SERIALBAUDRATE);
  }
  PRINTLNS("MqttNow object created");
#endif
};

void MqttNowBase::begin() {
  PRINTF("MqttNow %s started\n", FIRMWARE_VERSION);
  #ifdef M5STACK_FIRE
  M5.begin();
  M5.Power.begin();
  #endif
  #ifdef HAS_DISPLAY
  setupDisplay();
  #endif
  #ifdef DEBUGLOG
  echoFirmwareInfo();
  #endif
};

void MqttNowBase::update() {
  // if (serverRunning) {
  //   server.handleClient();
  // }
  #if (!(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))) | defined(MQTT_TEST_COMPILE)
  // Bridge has its own serial input handling
    #ifdef DEBUGLOG
      while (Serial.available()) {
        char c = Serial.read();
        if (c == 10 || c == 13) {
          echoFirmwareInfo();
        }
      }
    #endif
  #endif

  #ifdef M5STACK_FIRE
  M5.update();
  #endif

  #ifdef HAS_DISPLAY
  updateDisplay();
  #endif
};

void MqttNowBase::setName(char* name) {
  _name = String(name);
}

void MqttNowBase::setType(ComponentType type) {
  _type = type;
}

String MqttNowBase::getName() {
  return _name;
}

void MqttNowBase::getMac(uint8_t *macaddr) {
  getmac(macaddr);
}

ComponentType MqttNowBase::getType() {
  return _type;
}

void MqttNowBase::startOTA() {
  startWifi();
  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  startServer();
  PRINTLNS("OTA started.");
  PRINTS("Go to http://");
  PRINTDS(WiFi.localIP());
  PRINTLNS("/update");
}

void MqttNowBase::stopOTA() {
  // server.stop();
  server.end();
  if (stopWifiAfterOta) {
    stopWifi();
  }
}

void MqttNowBase::startWifi() {
  if (WiFi.status() == WL_CONNECTED) return;

  delay(10);
  // We start by connecting to a WiFi network

  PRINTS("connecting to Wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PW);
  while (WiFi.status() != WL_CONNECTED) {
    PRINTS(".");
    delay(200);
  }

  PRINTLNS("Succeeded")
  PRINTF("IP address: %s", WiFi.localIP().toString().c_str()) PRINTLF
  PRINTF("MAC: %s", WiFi.macAddress().c_str()) PRINTLF
  #ifdef HAS_DISPLAY
  log2Display("Connected to WiFI");
  log2Display(WiFi.localIP().toString().c_str());
  #endif
}

void MqttNowBase::stopWifi() {
  stopServer();
  if (WiFi.status() == WL_CONNECTED) {
    WiFi.disconnect();
  }
}

void MqttNowBase::startServer(){
  server.begin();
  serverRunning = true;
}

void MqttNowBase::stopServer(){
  serverRunning = false;
}

#ifdef DEBUGLOG
void MqttNowBase::echoFirmwareInfo() {
  PRINTF("Platform: %s", FIRMWARE_TARGET) PRINTLF;
  PRINTF("Node ID: %s", getNodeId().c_str()) PRINTLF;
  PRINTF2("Firmware: %s v%s", FIRMWARE_NAME, FIRMWARE_VERSION) PRINTLF;
  PRINTLNS("Build flags:")
  PRINTDS("MQTT_NOW_CLIENT: ")
  #ifdef MQTT_NOW_CLIENT
    PRINTLNS("true")
  #else
    PRINTLNS("false")
  #endif
  PRINTDS("MQTT_NOW_CONTROLLER: ")
  #ifdef MQTT_NOW_CONTROLLER
    PRINTLNS("true")
  #else
    PRINTLNS("false")
  #endif
}
#endif

String MqttNowBase::getNodeId() {
  char id[14];
  if (_id[0] != 'M') {
    uint8_t mac[6];
    
    getmac(&mac[0]);
    sprintf(_id, "MN%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  }
  strcpy(id, _id);
  return id;
}

Node MqttNowBase::getNodeStruct() {
  Node me;
  me.component = getType();
  me.id = getNodeId();
  me.name = getName();
  return me;
}

result_t MqttNowBase::_handleReboot() {
  delay(500);
  ESP.restart();
  return result_success;
}