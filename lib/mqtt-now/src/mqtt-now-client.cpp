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

// MqttNowClient::MqttNowClient() {
//   MqttNowClient(MQTT_HOST);
// };

/**
 * @brief Construct a new MqttNowClient object
 * 
 * @param host Hostname of the host running the MQTT broker
 * @param port Port of the MQTT broker (default 1883)
 */
// MqttNowClient::MqttNowClient(String host, uint16_t port) {
//   MqttNowClient(host, "", port);
// }

// /**
//  * @brief Construct a new MqttNowClient object
//  * 
//  * @param host Hostname of the host running the MQTT broker
//  * @param rootTopic 
//  * @param port Port of the MQTT broker (default 1883)
//  */
// MqttNowClient::MqttNowClient(String host, String rootTopic, uint16_t port) {
//   MqttNowClient(host, rootTopic, port, MQTT_CMD_TOPIC, MQTT_STATUS_TOPIC, MQTT_LWT_TOPIC,
//      MQTT_ON_CMD, MQTT_OFF_CMD, MQTT_ONLINE_LWT, MQTT_OFFLINE_LWT);
// }

MqttNowClient::MqttNowClient(
      String host, 
      uint16_t mqttPort,
      String rootTopic, 
      String cmdTopic, 
      String statusTopic, 
      String lwtTopic, 
      String onCmd, 
      String offCmd, 
      String onlineLwt,
      String offlineLwt) : MqttNowBase() {
  // TODO: Fix this
  //_host = host;
  PRINTLN("Constructing MqttNowClient with host ", host);
  _host = MQTT_HOST;
  PRINTLN("Constructing MqttNowClient with _host ", _host);
  _mqttPort = mqttPort;

  _mqttId = MQTT_ID;
  _mqttUser = MQTT_USER;
  _mqttPwd = MQTT_PW;

  if (rootTopic.length() > 0) {
    setRootTopic(rootTopic);
    setCmdTopic(_rootTopic + "/" + cmdTopic);
    setstatusTopic(_rootTopic + "/" + statusTopic);
    setLwtTopic(_rootTopic + "/" + lwtTopic);
    setOnCmd(onCmd);
    setOffCmd(offCmd);
    setOnlineLwt(onlineLwt);
    setOfflineLwt(offlineLwt);
  }
}

result_t MqttNowClient::start() {
  WiFiClient wifiClient;
  PubSubClient mqttClient(wifiClient);
  unsigned long connectStart = millis();
  
  PRINTLNS("connecting to mqtt broker host...");
  PRINTLN("Host: ", _host);
  PRINTLN("Post: ", _mqttPort);
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
  _mqttClient = &mqttClient;

  return start(&wifiClient);
}

result_t MqttNowClient::start(WiFiClient* wifiClient) {
  //MQTTPubSubClient mqttClient;
  //PubSubClient mqttClient(*wifiClient);
  unsigned long connectStart = millis();

  //mqttClient.begin(*wifiClient);
  //connect(const char *id, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage) {
  //mqttClient.connect()

  PRINTLNS("connecting to mqtt broker...");
  PRINTLN("ID: ", _mqttId);
  PRINTLN("User: ", _mqttUser);
  PRINTLN("PW: ", _mqttPwd);

  while ((!_mqttClient->connect(_mqttId.c_str(), _mqttUser.c_str(), _mqttPwd.c_str())) && (millis() - connectStart < 5000)){
    PRINTS(".");
    delay(200);
  }

  if (!_mqttClient->connected()) {
    PRINTLNS(" Failed!");
    return false;
  }

  PRINTLNS(" Success");

  // if (_rootTopic.length() > 0) {
  //   PRINTLNS("Setting LWT and current status");
  //   mqttClient.setWill(_lwtTopic, _offlineLwt);
  //   publish(_lwtTopic, _onlineLwt, true, 0);
  // }
  //_mqttClient = &mqttClient;

  return true;
}

/** To be called from void setup() **/
void MqttNowClient::begin() {
  MqttNowBase::begin();
  delay(100);

  if (!COM) {
    COM.begin(SERIALBAUDRATE);
  }

  PRINTLNS("Booting MqttNow Client");
  // WiFiManager wifiManager;
  // //reset settings - for testing
  // // wifiManager.resetSettings();

  // //wifiManager.autoConnect(AP_NAME, AP_PASSWORD);
  // wifiManager.autoConnect(AP_NAME);

  PRINTS("connecting to Wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PW);
  while (WiFi.status() != WL_CONNECTED) {
    PRINTS(".");
    delay(200);
  }

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
  //if(_mqttClient && _mqttClient->isConnected()) {
  //if(_mqttClient) {
    _mqttClient->loop();
  //}

  while (COM.available()) {
    char c = COM.read();
    if (c == 10 || c == 13) {
      _handleComm();
      _comBuff = "";
    } else {
      _comBuff += c;
    }
  }
};

/** Serial communication **/

/**
 * @brief Handle incomming serial communication
 * 
 * @param comm 
 * @return mqttnow_result 
 */
result_t MqttNowClient::_handleComm() {
  PRINTLN("Communication received: ", _comBuff);

  if (!_comBuff.startsWith(MSG_START)) {
    PRINTLNS("Unknown communication");
    return result_error;
  }

  char c = _comBuff.charAt(3);

  switch (c) {
    case MSG_ACTIONSUB:
      PRINTLNS("Subscribe command received");
      return _handleSubscription();
    case MSG_ACTIONPUB:
      PRINTLNS("Publish command received");
      return _handlePublish();
    default:
      PRINTLNS("Unknown command received");
      return result_error;
  }
}

result_t MqttNowClient::_handleSubscription() {
  String c = _comBuff.substring(4);
  return result_error;
}

result_t MqttNowClient::_handlePublish() {
  String c = _comBuff.substring(4);
  return publish(_statusTopic, c, true, 1);
}

/** Publishing **/

/**
 * @brief Publish a new status to the status topic
 * 
 * @param status The status to be published
 * @return result_t 
 */
result_t MqttNowClient::publishStatus(String status) {
  return publish(_statusTopic, status, true);
}

/**
 * @brief Publish a new command to the command topic
 * 
 * @param status The command to be published
 * @return result_t 
 */
result_t MqttNowClient::publishCmd(String cmd) {
  return publish(_cmdTopic, cmd, false, (uint8_t)1U );
}

/**
 * @brief Publish a new message to the specified topic
 * 
 * @param String topic The topic to publish to
 * @param String payload The payload (message) to be published
 * @param bool retain default = false
 * @param uint8_t qos  default 1
 * @return result_t 
 */
result_t MqttNowClient::publish(String topic, String payload, bool retain, uint8_t qos) {
  //return _mqttClient->publish(topic, payload, retain, qos);
  PRINTLN("Publishing to ", topic);
  PRINTLN("Payload: ", payload);
  return _mqttClient->publish(topic.c_str(), payload.c_str(), retain);
}


/** Simple setters **/

void MqttNowClient::setRootTopic(String rootTopic) { 
  _rootTopic = rootTopic; 
  PRINTLN("Root topic set to ", _rootTopic);
}

void MqttNowClient::setCmdTopic(String cmdTopic) { 
  _cmdTopic = cmdTopic; 
  PRINTLN("Command topic set to ", _cmdTopic);
}

void MqttNowClient::setstatusTopic(String statusTopic) {
  _statusTopic = statusTopic; 
  PRINTLN("Status topic set to ", _statusTopic);
}

void MqttNowClient::setLwtTopic(String lwtTopic) {
  _lwtTopic = lwtTopic; 
  PRINTLN("LWT topic set to ", _lwtTopic);
}

void MqttNowClient::setOnCmd(String onCmd) {
  _onCmd = onCmd; 
  PRINTLN("ON command set to ", _onCmd);
}

void MqttNowClient::setOffCmd(String offCmd) {
  _offCmd = offCmd; 
  PRINTLN("OFF command set to ", _offCmd);
}

void MqttNowClient::setOnlineLwt(String onlineLwt) {
  _onlineLwt = onlineLwt; 
  PRINTLN("Online LWT payload set to ", _onlineLwt);
}

void MqttNowClient::setOfflineLwt(String offlineLwt) {
  _offlineLwt = offlineLwt; 
  PRINTLN("Offline LWT payload set to ", _offlineLwt);
}

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