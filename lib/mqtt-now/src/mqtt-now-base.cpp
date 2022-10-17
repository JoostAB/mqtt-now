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
  PRINTLNS("MqttNow started");
};

void MqttNowBase::update() {
  // if (serverRunning) {
  //   server.handleClient();
  // }
};

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

  PRINTS("connected as :");
  PRINTLNSA(WiFi.localIP());
  PRINTS("MAC :");
  PRINTLNSA(WiFi.macAddress());
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