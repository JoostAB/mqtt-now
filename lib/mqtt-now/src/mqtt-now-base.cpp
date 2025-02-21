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
  void getmac(uint8_t *macaddr) {esp_wifi_get_mac(WIFI_IF_STA, macaddr);}
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
};

void MqttNowBase::update() {
  // if (serverRunning) {
  //   server.handleClient();
  // }
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
  static uint32_t retrycount = 0;
  if (WiFi.status() == WL_CONNECTED) return;

  delay(10);
  // We start by connecting to a WiFi network

  PRINTS("connecting to Wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PW);
  while (WiFi.status() != WL_CONNECTED) {
    retrycount ++;
    if (retrycount > 50) {
      PRINTLNS("Tried for 10 seconds... Rebooting");
      delay(200);
      ESP.restart();
    }
    PRINTS(".");
    delay(200);
  }
  retrycount = 0;
  PRINTS("connected as :");
  PRINTLNSA(WiFi.localIP());
  PRINTS("MAC :");
  PRINTLNSA(WiFi.macAddress());
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
  server.end();
  serverRunning = false;
}

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