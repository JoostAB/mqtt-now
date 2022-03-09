/**
 * @file mqtt-now-client.cpp
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <mqtt-now-client.h>
#ifdef MQTT_NOW_CLIENT

ESP8266WebServer server(80);

/** Constructors **/

MqttNowClient::MqttNowClient() {
  MqttNowClient(MQTT_HOST, MQTT_PORT);
};

/**
 * @brief Construct a new MqttNowClient object
 * 
 * @param host Hostname of the host running the MQTT broker
 * @param port Port of the MQTT broker (default 1883)
 */
MqttNowClient::MqttNowClient(String host, uint16_t port) {
  MqttNowClient(host, "", port);
}

/**
 * @brief Construct a new MqttNowClient object
 * 
 * @param host Hostname of the host running the MQTT broker
 * @param rootTopic 
 * @param port Port of the MQTT broker (default 1883)
 */
MqttNowClient::MqttNowClient(String host, String rootTopic, uint16_t port) {
  MqttNowClient(host, rootTopic, MQTT_CMD_TOPIC, MQTT_STATUS_TOPIC, MQTT_LWT_TOPIC,
     MQTT_ON_CMD, MQTT_OFF_CMD, MQTT_ONLINE_LWT, MQTT_OFFLINE_LWT, port);
}

MqttNowClient::MqttNowClient(
      String host, 
      String rootTopic, 
      String cmdTopic, 
      String statusTopic, 
      String lwtTopic, 
      String onCmd, 
      String offCmd, 
      String onlineLwt,
      String offlineLwt, 
      uint16_t mqttPort) : MqttNowBase() {
  _host = host;
  _mqttPort = mqttPort;

  /* TODO: fill with useful stuff */
  _mqttId = "something";
  _mqttUser = "mqtt";
  _mqttPwd = "mqtt_user";

  if (rootTopic.length() > 0) {
    setRootTopic(rootTopic);
    setCmdTopic(_rootTopic + "\\" + cmdTopic);
    setstatusTopic(_rootTopic + "\\" + statusTopic);
    setLwtTopic(_rootTopic + "\\" + lwtTopic);
    setOnCmd(onCmd);
    setOffCmd(offCmd);
    setOnlineLwt(onlineLwt);
    setOfflineLwt(offlineLwt);
  }
}

mqttnow_success MqttNowClient::start() {
  WiFiClient wifiClient;
  unsigned long connectStart = millis();
  
  PRINTLNS("connecting to mqtt broker host...");

  #ifdef ESP32
    while ((!wifiClient.connect(_host.c_str(), _mqttPort)) && (millis() - connectStart < _timeout)) {
  #elif defined(ESP8266)
    while ((!wifiClient.connect(_host, _mqttPort)) && (millis() - connectStart < _timeout)) {
  #endif
    PRINTS(".");
    delay(200);
  }

  if (!wifiClient.connected()) {
    PRINTLNS("Failed!");
    return false;
  }

  _wifiClient = &wifiClient;

  return start(&wifiClient);
}

mqttnow_success MqttNowClient::start(WiFiClient* wifiClient) {
  MQTTPubSubClient mqttClient;
  unsigned long connectStart = millis();

  mqttClient.begin(*wifiClient);

  PRINTLNS("connecting to mqtt broker...");

  while ((!mqttClient.connect(_mqttId, _mqttUser, _mqttPwd)) && (millis() - connectStart < 5000)){
    PRINTS(".");
    delay(200);
  }

  if (!mqttClient.isConnected()) {
    PRINTLNS(" Failed!");
    return false;
  }

  PRINTLNS(" Success");

  if (_rootTopic.length() > 0) {
    PRINTLNS("Setting LWT and current status");
    mqttClient.setWill(_lwtTopic, _offlineLwt);
    publish(_lwtTopic, _onlineLwt, true, 0);
  }
  _mqttClient = &mqttClient;

  return true;
}

/** To be called from void setup() **/
void MqttNowClient::begin() {
  MqttNowBase::begin();
  delay(100);

  PRINTLNS("Booting MqttNow Client");
  WiFiManager wifiManager;
  //reset settings - for testing
  // wifiManager.resetSettings();

  //wifiManager.autoConnect(AP_NAME, AP_PASSWORD);
  wifiManager.autoConnect(AP_NAME);

  PRINTS("connected as :");
  PRINTLNS(WiFi.localIP());
  PRINTS("MAC :");
  PRINTLNS(WiFi.macAddress());

  _startWebserver();
  
  _startMqttClient();
};

void MqttNowClient::_startWebserver() {
  PRINTLNS("Setting up webserver");
  server.onNotFound(MqttNowClient::handleCommand);

  server.begin();
  PRINTLNS("Webserver started");
}
void MqttNowClient::_startMqttClient() {
  // TODO: for now this way. Has to be improved
  start();
}

/** To be called from void loop() **/
void MqttNowClient::update() {
  MqttNowBase::update();
  server.handleClient();
  if(_mqttClient && _mqttClient->isConnected()) {
    _mqttClient->update();
  }
};

/** Publishing **/

/**
 * @brief Publish a new status to the status topic
 * 
 * @param status The status to be published
 * @return success_t 
 */
mqttnow_success MqttNowClient::publishStatus(String status) {
  return publish(_statusTopic, status, true);
}

/**
 * @brief Publish a new command to the command topic
 * 
 * @param status The command to be published
 * @return success_t 
 */
mqttnow_success MqttNowClient::publishCmd(String cmd) {
  return publish(_cmdTopic, cmd, false, (uint8_t)1U );
}

/**
 * @brief Publish a new message to the specified topic
 * 
 * @param String topic The topic to publish to
 * @param String payload The payload (message) to be published
 * @param bool retain default = false
 * @param uint8_t qos  default 0
 * @return success_t 
 */
mqttnow_success MqttNowClient::publish(String topic, String payload, bool retain, uint8_t qos) {
  return _mqttClient->publish(topic, payload, retain, qos);
}


/** Simple setters **/

void MqttNowClient::setRootTopic(String rootTopic) { _rootTopic = rootTopic; }

void MqttNowClient::setCmdTopic(String cmdTopic) { _cmdTopic = cmdTopic; }

void MqttNowClient::setstatusTopic(String statusTopic) { _statusTopic = statusTopic; }

void MqttNowClient::setLwtTopic(String lwtTopic) { _lwtTopic = lwtTopic; }

void MqttNowClient::setOnCmd(String onCmd) { _onCmd = onCmd; }

void MqttNowClient::setOffCmd(String offCmd) { _offCmd = offCmd; }

void MqttNowClient::setOnlineLwt(String onlineLwt) { _onlineLwt = onlineLwt; }

void MqttNowClient::setOfflineLwt(String offlineLwt) { _offlineLwt = offlineLwt; }

/** Webserver handlers **/

void MqttNowClient::handleUpdate() {
  server.send(200, "text/plain", "Here comes the OTA update page");
}
void MqttNowClient::handleCommand() {
   String cmd = server.uri();
   if (cmd.equals("/")) {
     server.send(200, "text/plain", "Welcome to MQTTNow bridge");
   } else if (cmd.equals("/update")) {
     MqttNowClient::handleUpdate();
   } else {
     MqttNowClient::handleNotFound();
   }
}

void MqttNowClient::handleNotFound() {
    server.send(404, "text/plain", "404: File Not Found");
}




#endif // MQTT_NOW_CLIENT