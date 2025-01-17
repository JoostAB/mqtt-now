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
#if defined(MQTT_NOW_CLIENT) | defined(MQTT_TEST_COMPILE)

bool mqttReceived = false;
String lastReceivedTopic;
String lastReceivedPayload;

void mqttMsgReceived(char* topic, byte* payload, unsigned int length) {
  PRINTF("Message arrived [%s]: ", topic)
  lastReceivedTopic = String(topic);
  lastReceivedPayload = "";
  for (unsigned int i = 0; i < length; i++) {
    lastReceivedPayload += (char)payload[i];
  }
  PRINTLNSA(lastReceivedPayload);
  mqttReceived = true;
}

/** Constructors **/

/**
 * @brief Construct a new MqttNowClient object
 * 
 */
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

WiFiClient espClient;
PubSubClient client(espClient);

/**
 * @brief Construct a new Mqtt Now Client:: Mqtt Now Client object
 * 
 * @param host Hostname of the host running the MQTT broker
 * @param port Port of the MQTT broker (default 1883)
 * @param rootTopic 
 * @param cmdTopic 
 * @param statusTopic 
 * @param lwtTopic 
 * @param devTopic 
 * @param onCmd 
 * @param offCmd 
 * @param onlineLwt 
 * @param offlineLwt 
 */
MqttNowClient::MqttNowClient(
      String host, 
      uint16_t mqttPort,
      String rootTopic, 
      String cmdTopic, 
      String statusTopic, 
      String lwtTopic, 
      String devTopic, 
      String sysinfoTopic,
      String onCmd, 
      String offCmd, 
      String onlineLwt,
      String offlineLwt) : MqttNowBridge() {
  _host = MQTT_HOST;
  _mqttPort = mqttPort;

  _mqttId = MQTT_ID;
  _mqttUser = MQTT_USER;
  _mqttPwd = MQTT_PW;

  if (rootTopic.length() > 0) {
    _rootTopic = rootTopic; 
    _cmdTopic = _rootTopic + PATHSEP + cmdTopic;
    _statusTopic = _rootTopic + PATHSEP + statusTopic;
    _lwtTopic = _rootTopic + PATHSEP + lwtTopic;
    _devTopic = _rootTopic + PATHSEP + devTopic;
    _sysinfoTopic = _rootTopic + PATHSEP + sysinfoTopic;
    _onCmd = onCmd;
    _offCmd = offCmd;
    _onlineLwt = onlineLwt;
    _offlineLwt = offlineLwt;
  }

  PRINTLNS("Topics set");

  //client.setBufferSize(1024);
  client.setBufferSize(4096);

  stopWifiAfterOta = false;
}

/**
 * @brief To be called from main setup
 * 
 */
void MqttNowClient::begin() {
  MqttNowBase::begin();

  if (!COM) {
    COM.begin(SERIALBAUDRATE);
  }

  //_setupWifi();
  startWifi();
 
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(mqttMsgReceived);
};

void MqttNowClient::_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    PRINTLNS("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_ID, MQTT_USER, MQTT_PW, _lwtTopic.c_str(), 1, true, _offlineLwt.c_str())) {
      PRINTLNSA("connected to " + _host);
      // Once connected, LWT Online message...
      publish(_lwtTopic.c_str(), _onlineLwt.c_str(), true);
      publishSysInfo();
      //makeDiscoverable();
      setupAutoDiscover();
      // ... and resubscribe to command topic
      client.subscribe(_cmdTopic.c_str());
    } else {
      PRINTDS("failed, rc=");
      PRINTDS(client.state());
      PRINTDS(" try again in 5 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

void MqttNowClient::_setupWifi() {

  // delay(10);
  // // We start by connecting to a WiFi network

  // PRINTS("connecting to Wifi...");
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(WIFI_SSID, WIFI_PW);
  // while (WiFi.status() != WL_CONNECTED) {
  //   PRINTS(".");
  //   delay(200);
  // }

  // PRINTS("connected as :");
  // PRINTLNSA(WiFi.localIP());
  // PRINTS("MAC :");
  // PRINTLNSA(WiFi.macAddress());
}

/**
 * @brief To be called from main loop
 * 
 */
void MqttNowClient::update() {
  //MqttNowBase::update();

  // Listen to serial port
  MqttNowBridge::update();

  // Now check if a message was received from the broker
  if (mqttReceived) {
    if (lastReceivedTopic.equals(_lastPublishedTopic) && lastReceivedPayload.equals(_lastPublishedPayload)) {
      // Self send message, so do nothing with it but clear buffers
      PRINTLN("Received our own message on ", lastReceivedTopic);
      _lastPublishedPayload.clear();
      _lastPublishedPayload.clear();
    } else if (lastReceivedTopic.equals(_cmdTopic)) {
      // A message on the command topic is meant for the client itself
      _handleCommand();
    } else {
      // All other messages are for a node in the ESP-NOW network, 
      // so let the controller handle that
      _sendMqttMsgToController();
    }
    mqttReceived = false;
  }

  // Handle MQTT stuff
  if (client.connected()) {
    client.loop();
  } else {
    _reconnect();
  }
};

/** Serial communication **/

/**
 * @brief Handle incomming serial communication
 * 
 * @param comm 
 * @return mqttnow_result 
 */
// result_t MqttNowClient::_handleComm() {
//   PRINTLN("Communication received: ", _comBuff);
//   #ifdef HAS_DISPLAY
//   log2Display(("IN: "+_comBuff).c_str());
//   #endif
//   if (!_comBuff.startsWith(MSG_START)) {
//     PRINTLNS("Unknown communication");
//     return result_error;
//   }

//   // Get character after MSG_START (the action tag)
//   char act = _comBuff.charAt(3);
//   return _doAction(act);
// }

result_t MqttNowClient::_doAction(char act) {
  switch (act) {
    case MSG_ACTIONSUB:
      PRINTLNS("Subscribe command received");
      return _handleSubscribe();
    case MSG_ACTIONUNS:
      PRINTLNS("Unsubscribe command received");
      return _handleUnsubscribe();
    case MSG_ACTIONPUB:
      PRINTLNS("Publish command received");
      return _handlePublish();
    case MSG_ACTIONRBT:
      PRINTLNS("Reboot command received");
      return _handleReboot();
    default:
      PRINTLNS("Unknown command received");
      return result_error;
  }
}

result_t MqttNowClient::_handleSubscribe() {
  // Everything after action tag is the topic to subscribe to
  char qos = _comBuff.charAt(4);
  String topic = _modTopic(_comBuff.substring(5));
  PRINTF2("Subscribing to topic: %s with QOS %c", topic.c_str(), qos) PRINTLF;

  #ifdef HAS_DISPLAY
  log2Display(("Subscribing to: " + topic).c_str());
  #endif

  if (!client.subscribe(topic.c_str(), qos - '0')) {
    PRINTLNS("Error during subscribing");
    return result_error;
  } 
  return result_success;  
}

result_t MqttNowClient::_handleUnsubscribe() {
  String topic = _modTopic(_comBuff.substring(4));
  PRINTLN("Unsubscribing from topic: ", topic.c_str());

  #ifdef HAS_DISPLAY
  log2Display(("Unsubscribing from: " + topic).c_str());
  #endif

  if (!client.unsubscribe(topic.c_str())) {
    PRINTLNS("Error during un-subscribing");
    return result_error;
  } 
  return result_success;  
}

result_t MqttNowClient::_handlePublish(String com) {
  static uint8_t ctr = 0;

  if (com.equals("")) {
    com = String(_comBuff.c_str());
  }
  char qos = com.charAt(4);
  char retained = com.charAt(5);
  PRINTLN("QOS = ", qos);
  PRINTLN("Retained = ", retained);
  
  int payloadPos = com.lastIndexOf(MSG_PAYLOAD);
  String topic = _modTopic(com.substring(6,payloadPos));
  String payload = com.substring(payloadPos + 3);

  PRINTLN("Topic = ", topic);
  PRINTLN("payload = ", payload);

  // Try max 5 times to publish this
  if (publish(topic, payload, (retained == '1'), (qos - '0')) == result_success) {
    PRINTLNS("Publish ok");
    ctr = 0;
    return result_success;
  } else {
    if (ctr < 5) {
      ctr ++;
      yield();
      PRINTLN("Publish attempt nr ", (ctr+1));
      return _handlePublish(com);
    } else {
      // Reached max of 5 attempts, return error
      PRINTLNS("Error publishing message");
      ctr = 0;
      return result_error;
    }
  }
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
  PRINTLN("Publishing to ", topic);
  PRINTLN("Payload: ", payload);
  
  #ifdef HAS_DISPLAY
  log2Display(("Publishing to: " + topic).c_str());
  #endif

  _lastPublishedTopic = topic;
  _lastPublishedPayload = payload;
  return client.publish(topic.c_str(), payload.c_str(), retain);
}

result_t MqttNowClient::publishSysInfo() {
  JsonDocument doc;
  JsonObject netw = doc["network"].to<JsonObject>();
  netw["ip_address"] = WiFi.localIP();
  netw["hostname"] = WiFi.getHostname();
  
  doc.shrinkToFit();

  String json;

  serializeJson(doc, json);
  return publish(_sysinfoTopic, json, true, 1);
}

result_t MqttNowClient::makeDiscoverable() {
  return makeDiscoverable(getNodeStruct());
}

result_t MqttNowClient::makeDiscoverable(Node node) {
  return result_error;
}

result_t MqttNowClient::_handleCommand() {
  PRINTLN("Performing command: ", lastReceivedPayload);
  if (lastReceivedPayload.equals(CMD_OTA_START)) {
    startOTA();
    return result_success;
  }

  if (lastReceivedPayload.equals(CMD_OTA_STOP)) {
    stopOTA();
    return result_success;
  }
  
  if (lastReceivedPayload.equals(CMD_CTRL_REBOOT)) {
    return _sendStringToController("###B");
  }

  if (lastReceivedPayload.equals(CMD_CLIENT_REBOOT)) {
    return _handleReboot();
  }

  // Unknown command
  return result_error;
}

result_t MqttNowClient::_handleReboot() {
  delay(500);
  ESP.restart();
  return result_success;
}

result_t MqttNowClient::_sendStringToController(const char* msg) {
  if (!COM) {
    COM.begin(SERIALBAUDRATE);
    yield();
  }
  PRINTS("Sending to controller: ");
  PRINTLNSA(msg);
  #ifdef HAS_DISPLAY
  log2Display("OUT:");
  log2Display(msg);
  #endif
  size_t send = 0;
  send += COM.print(msg);
  COM.print("\n");
  PRINTLN("Nr of bytes send to controller: ", send);
  return (send > 0)?result_success:result_error;
}

result_t MqttNowClient::_sendMqttMsgToController() {
  PRINTS("Sending to controller: ");

  if (!COM) {
    COM.begin(SERIALBAUDRATE);
    yield();
  }
    
  PRINTS(MSG_START);
  PRINTDS(MSG_ACTIONREC);
  PRINTDS(lastReceivedTopic);
  PRINTS(MSG_PAYLOAD);
  PRINTLNSA(lastReceivedPayload);
  
  #ifdef HAS_DISPLAY
  log2Display("OUT:");
  log2Display(lastReceivedTopic.c_str());
  log2Display(lastReceivedPayload.c_str());
  #endif
  
  size_t send = 0;
  send += COM.print(MSG_START);
  send += COM.print(MSG_ACTIONREC);
  send += COM.print(lastReceivedTopic);
  send += COM.print(MSG_PAYLOAD);
  send += COM.print(lastReceivedPayload);
  COM.print("\n");
  return (send > 0)?result_success:result_error;
}

/** Simple setters **/

void MqttNowClient::setDiscoveryTopic(String discoveryTopic) { 
  _discoveryTopic = discoveryTopic; 
  PRINTLN("Discovery topic set to ", _discoveryTopic);
}

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

void MqttNowClient::setDevicesTopic(String devTopic) {
  _devTopic = devTopic; 
  PRINTLN("Devices topic set to ", _devTopic);
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

// void MqttNowClient::handleUpdate() {
//   server.send(200, "text/plain", "Here comes the OTA update page");
// }
// void MqttNowClient::handleCommand() {
//    String cmd = server.uri();
//    if (cmd.equals("/")) {
//      server.send(200, "text/plain", "Welcome to MQTTNow bridge");
//    } else if (cmd.equals("/update")) {
//      MqttNowClient::handleUpdate();
//    } else {
//      MqttNowClient::handleNotFound();
//    }
// }

// void MqttNowClient::handleNotFound() {
//     server.send(404, "text/plain", "404: File Not Found");
// }

#ifdef HASS_AUTODISCOVER
result_t MqttNowClient::setupAutoDiscover() {
  setADaddress();
  setADhostname();
  setADstate();
  return result_success;
}

result_t MqttNowClient::setADhostname() {
  String path = _discoveryTopic + "/sensor/mqtt-now-" + getNodeId() + "/hostname/config";

  JsonDocument doc;
  
  JsonObject dev = doc["device"].to<JsonObject>();
  dev["hw"] = HARDWARE_VERSION;
  dev["ids"] = "mqttnow_bridge_" + getNodeId();
  dev["mf"] = "MQTT-NOW";
  dev["mdl"] = "Bridge";
  dev["name"] = "MQTT-NOW Bridge";
  dev["sw"] = FIRMWARE_VERSION;

  JsonObject origin = doc["origin"].to<JsonObject>();
  origin["name"] = "MQTT-NOW";
  origin["sw"] = "MQTT-NOW " + String(FIRMWARE_VERSION);

  doc["entity_category"] = "diagnostic";
  
  doc["avty_t"] = _lwtTopic,
  doc["pl_avail"] = _onlineLwt;
  doc["pl_not_avail"] = _offlineLwt;

  doc["icon"] = "mdi:network";
  doc["name"] = "Hostname";
  
  doc["stat_t"] = _sysinfoTopic;
  doc["unique_id"] = "mqttnow_bridge_" + getNodeId() + "_hostname";
  doc["object_id"] = doc["unique_id"];
  doc["val_tpl"] = "{{ value_json.network.hostname }}";
  doc.shrinkToFit();

  String json;

  serializeJson(doc, json);
  PRINTLN("setADaddress: ",json);
  return publish(path, json, true, 1);

}

result_t MqttNowClient::setADaddress() {
  String path = _discoveryTopic + "/sensor/mqtt-now-" + getNodeId() + "/ip-address/config";

  JsonDocument doc;
  
  JsonObject dev = doc["device"].to<JsonObject>();
  dev["hw"] = HARDWARE_VERSION;
  dev["ids"] = "mqttnow_bridge_" + getNodeId();
  dev["mf"] = "MQTT-NOW";
  dev["mdl"] = "Bridge";
  dev["name"] = "MQTT-NOW Bridge";
  dev["sw"] = FIRMWARE_VERSION;

  JsonObject origin = doc["origin"].to<JsonObject>();
  origin["name"] = "MQTT-NOW";
  origin["sw"] = "MQTT-NOW " + String(FIRMWARE_VERSION);

  doc["entity_category"] = "diagnostic";
  
  doc["avty_t"] = _lwtTopic,
  doc["pl_avail"] = _onlineLwt;
  doc["pl_not_avail"] = _offlineLwt;

  doc["icon"] = "mdi:ip";
  doc["name"] = "WiFi IP address";
  
  doc["stat_t"] = _sysinfoTopic;
  doc["unique_id"] = "mqttnow_bridge_" + getNodeId() + "_connection_ip";
  doc["object_id"] = doc["unique_id"];
  doc["val_tpl"] = "{{ value_json.network.ip_address }}";
  doc.shrinkToFit();

  String json;

  serializeJson(doc, json);
  PRINTLN("setADaddress: ",json);
  return publish(path, json, true, 1);
}

result_t MqttNowClient::setADstate() {
  String path = _discoveryTopic + "/binary_sensor/mqtt-now-" + getNodeId() + "/state/config";
  
  JsonDocument doc;
  
  doc["entity_category"] = "diagnostic";

  JsonObject dev = doc["device"].to<JsonObject>();
  dev["hw"] = HARDWARE_VERSION;
  dev["ids"] = "mqttnow_bridge_" + getNodeId();
  dev["mf"] = "MQTT-NOW";
  dev["mdl"] = "Bridge";
  dev["name"] = "MQTT-NOW Bridge";
  dev["sw"] = FIRMWARE_VERSION;
  
  JsonObject origin = doc["origin"].to<JsonObject>();
  origin["name"] = "MQTT-NOW";
  origin["sw"] = "MQTT-NOW " + String(FIRMWARE_VERSION);

  doc["entity_category"] = "diagnostic";
  
  doc["stat_t"] = _lwtTopic;
  doc["payload_off"] = _offlineLwt;
  doc["payload_on"] = _onlineLwt;
  doc["unique_id"] = "mqttnow_bridge_" + getNodeId() + "_state";
  doc["object_id"] = doc["unique_id"];
  doc.shrinkToFit();

  String json;

  serializeJson(doc, json);
  return publish(path, json, true, 1);
}

result_t MqttNowClient::addDevice(JsonDocument* doc) {
  JsonObject docObject = (*doc).to<JsonObject>();
  return addDevice(&docObject);
}

result_t MqttNowClient::addDevice(JsonObject* obj) {
  JsonObject device = (*obj)["dev"].to<JsonObject>();
  device["hw"] = HARDWARE_VERSION;
  device["ids"][0] = "mqttnow_bridge_" + getNodeId();
  device["mf"] = "MQTT-NOW";
  device["mdl"] = "Bridge";
  device["name"] = "MQTT-NOW Bridge";
      
  device["sw"] = FIRMWARE_VERSION;
  
  return result_success;
}

JsonObject MqttNowClient::getDeviceObj() {
  JsonObject dev;
  dev["hw"] = HARDWARE_VERSION;
  dev["ids"][0] = "mqttnow_bridge_" + getNodeId();
  dev["mf"] = "MQTT-NOW";
  dev["mdl"] = "Bridge";
  dev["name"] = "MQTT-NOW Bridge";
      
  dev["sw"] = FIRMWARE_VERSION;
  return dev;
}

result_t MqttNowClient::addAvailability(JsonDocument* doc) {
  JsonObject docObject = (*doc).to<JsonObject>();
  return addAvailability(&docObject);
}

result_t MqttNowClient::addAvailability(JsonObject* obj) {
  (*obj)["avty_t"] = _lwtTopic,
  (*obj)["pl_avail"] = _onlineLwt;
  (*obj)["pl_not_avail"] = _offlineLwt;
  return result_success;
}

result_t MqttNowClient::addOrigin(JsonDocument* doc) {
  JsonObject docObject = (*doc).to<JsonObject>();
  return addOrigin(&docObject);
}

result_t MqttNowClient::addOrigin(JsonObject* obj) {
  JsonObject origin = (*obj)["origin"].to<JsonObject>();
  origin["name"] = "MQTT-NOW";
  origin["sw"] = "MQTT-NOW " + String(FIRMWARE_VERSION);
  return result_success;
}

String MqttNowClient::_getFullDiscoveryPath(Node node) {
  return _discoveryTopic + PATHSEP + node.component + PATHSEP + node.id + PATHSEP + "config";
}
#endif
/** Utilities **/
String MqttNowClient::_modTopic(String topic) {
  String ret;
  switch (topic.charAt(0)) {
    case PATHSEP:
      ret = _devTopic + PATHSEP + topic.substring(1);
      break;
    case '@':
      ret = _rootTopic + PATHSEP + topic.substring(1);
      break;
    default:
      ret = topic;
      break;
  }
  return ret;
}
#endif // MQTT_NOW_CLIENT