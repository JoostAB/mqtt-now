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


void callback(char* topic, byte* payload, unsigned int length) {
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
    setCmdTopic(_rootTopic + "/" + cmdTopic);
    setstatusTopic(_rootTopic + "/" + statusTopic);
    setLwtTopic(_rootTopic + "/" + lwtTopic);
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
  client.setCallback(callback);
};

void MqttNowClient::_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // Attempt to connect
    if (client.connect(MQTT_ID, MQTT_USER, MQTT_PW)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(_lwtTopic.c_str(), "Online");
      // ... and resubscribe
      client.subscribe(_cmdTopic.c_str());
    } else {
      PRINTDS("failed, rc=");
      PRINTDS(client.state());
      PRINTDS(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
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

  // Tiny non-blocking delay
  randomSeed(micros());

  PRINTS("connected as :");
  PRINTLNS(WiFi.localIP());
  PRINTS("MAC :");
  PRINTLNS(WiFi.macAddress());
}

// void MqttNowClient::_callback(char* topic, byte* payload, unsigned int length) {
//   PRINTS("Message arrived [");
//   PRINTDS(topic);
//   PRINTS("] ");
//   for (int i = 0; i < length; i++) {
//     PRINTDS((char)payload[i]);
//   }
//   PRINTLF;

//   // Switch on the LED if an 1 was received as first character
//   if ((char)payload[0] == '1') {
//     digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
//     // but actually the LED is on; this is because
//     // it is active low on the ESP-01)
//   } else {
//     digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
//   }

// }


/** To be called from void loop() **/
void MqttNowClient::update() {
  MqttNowBase::update();
  while (COM.available()) {
    char c = COM.read();
    if (c == 10 || c == 13) {
      _handleComm();
      _comBuff = "";
    } else {
      _comBuff += c;
    }
  }

  if (!client.connected()) {
    _reconnect();
  }
  client.loop();
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


#endif // MQTT_NOW_CLIENT