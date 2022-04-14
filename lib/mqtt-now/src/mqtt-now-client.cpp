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


void mqttMsgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  }

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
      String onCmd, 
      String offCmd, 
      String onlineLwt,
      String offlineLwt) : MqttNowBase() {
  _host = MQTT_HOST;
  _mqttPort = mqttPort;

  _mqttId = MQTT_ID;
  _mqttUser = MQTT_USER;
  _mqttPwd = MQTT_PW;

  if (rootTopic.length() > 0) {
    setRootTopic(rootTopic);
    setCmdTopic(_rootTopic + PATHSEP + cmdTopic);
    setstatusTopic(_rootTopic + PATHSEP + statusTopic);
    setLwtTopic(_rootTopic + PATHSEP + lwtTopic);
    setDevicesTopic(_rootTopic + PATHSEP + devTopic);
    setOnCmd(onCmd);
    setOffCmd(offCmd);
    setOnlineLwt(onlineLwt);
    setOfflineLwt(offlineLwt);
  }
}

/** To be called from void setup() **/
void MqttNowClient::begin() {
  MqttNowBase::begin();
  pinMode(LED_BUILTIN, OUTPUT);

  if (!COM) {
    COM.begin(SERIALBAUDRATE);
  }

  _setupWifi();
 
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(mqttMsgReceived);
};

void MqttNowClient::_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    PRINTLNS("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(MQTT_ID, MQTT_USER, MQTT_PW, _lwtTopic.c_str(), 1, true, _offlineLwt.c_str())) {
      PRINTLNS("connected");
      // Once connected, LWT Online message...
      client.publish(_lwtTopic.c_str(), _onlineLwt.c_str(), true);
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

/** To be called from void loop() **/
void MqttNowClient::update() {
  MqttNowBase::update();

  // Store UART input in a local buffer, until
  // CR (13) or LF (10) is received
  while (COM.available()) {
    char c = COM.read();
    if (c == 10 || c == 13) {
      _handleComm();
      _comBuff = "";
    } else {
      _comBuff += c;
    }
  }

  // Handle MQTT stuff
  if (!client.loop()) {
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
result_t MqttNowClient::_handleComm() {
  PRINTLN("Communication received: ", _comBuff);

  if (!_comBuff.startsWith(MSG_START)) {
    PRINTLNS("Unknown communication");
    return result_error;
  }

  // Get character after MSG_START (the action tag)
  char act = _comBuff.charAt(3);

  switch (act) {
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
  // Everything after action tag is the topic to subscribe to
  String top = _comBuff.substring(4);
  PRINTLN("Subscribing to topic: ", top);
  if (client.subscribe(top.c_str())) {
    PRINTLNS("Error during subscribing");
    return result_error;
  } 
  return result_success;  
}

result_t MqttNowClient::_handlePublish(String com) {
  static uint8 ctr = 0;

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
    ctr = 0;
    return result_success;
  } else {
    if (ctr < 5) {
      yield();
      ctr ++;
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
  return client.publish(topic.c_str(), payload.c_str(), retain);
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

/** Utilities **/
String MqttNowClient::_modTopic(String topic) {
  
  switch (topic.charAt(0)) {
    case '/':
      return _devTopic + PATHSEP + topic.substring(1);
    case '@':
      return _rootTopic + PATHSEP + topic.substring(1);
    default:
      return topic;
  }
}
#endif // MQTT_NOW_CLIENT