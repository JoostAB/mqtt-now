#include <mqtt-now-client.h>
#ifdef MQTT_NOW_CLIENT

ESP8266WebServer server(80);

void handleUpdate() {
  server.send(200, "text/plain", "Here comes the OTA update page");
}
void handleCommand() {
   String cmd = server.uri();
   if (cmd.equals("/")) {
     server.send(200, "text/plain", "Welcome to MQTTNow bridge");
   } else if (cmd.equals("/update")) {
     handleUpdate();
   } else {
     handleNotFound();
   }
}

void handleNotFound() {
    server.send(404, "text/plain", "404: File Not Found");
}

void MqttNowClient::begin() {
  Serial.begin(115200); 
  delay(200);

  Serial.println("Booting MqttNow Client");
  WiFiManager wifiManager;
  //reset settings - for testing
  // wifiManager.resetSettings();

  //wifiManager.autoConnect(AP_NAME, AP_PASSWORD);
  wifiManager.autoConnect(AP_NAME);

  Serial.print("connected as :");
  Serial.println(WiFi.localIP());
  Serial.print("MAC :");
  Serial.println(WiFi.macAddress());

  Serial.println("Setting up webserver");
  server.onNotFound(handleCommand);

  server.begin();
  Serial.println("Webserver started");
};

void MqttNowClient::update() {
  server.handleClient();
};

// void MqttNowClient::handleUpdate() {
//   server.send(200, "text/plain", "Here comes the OTA update page");
// }
// void MqttNowClient::handleCommand() {
//    String cmd = server.uri();
//    if (cmd.equals("/")) {
//      server.send(200, "text/plain", "Welcome to MQTTNow bridge");
//    } else if (cmd.equals("/update")) {
//      this->handleUpdate();
//    } else {
//      this->handleNotFound();
//    }
// }

// void MqttNowClient::handleNotFound() {
//     server.send(404, "text/plain", "404: File Not Found");
// }

#endif // MQTT_NOW_CLIENT