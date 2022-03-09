#include <mqtt-now-node.h>
#if !defined(MQTT_NOW_CLIENT)

/**
 * @brief A random key that will be used for encryption
 */
uint8_t _lmk[] = 
  {0x84, 0x0D, 0x8E, 0xAB, 
  0xB9, 0x37, 0x8E, 0xAB,
  0x0A, 0xC4, 0x97, 0x53,
  0xE6, 0x12, 0x00, 0xAB};

// Broadcast address to all peers
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

struct_message myData;


esp_now_peer_info_t peerInfo;


/*************************/
/** OnDataSent Callback **/
/*************************/
#if defined(ESP32)
  void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
#elif defined(ESP8266)
  void OnDataSent(uint8_t *mac_addr, uint8_t status) {
#endif
  PRINTS("Last Packet Send Status: ");
  if (status == ESP_NOW_SEND_SUCCESS) {
    PRINTLN("Delivery ", "success");
  } else {
    PRINTLN("Delivery ", "fail")
  }
  
}

/*****************************/
/** onDataReceiver Callback **/
/*****************************/
 #ifdef ESP32
    void onDataReceiver(const uint8_t * mac, const uint8_t *incomingData, int len) {
  #elif defined(ESP8266)
    void onDataReceiver(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  #endif
  PRINTLNS("Message received.");
  memcpy(&myData, incomingData, sizeof(myData));
  
  PRINTLN("Bytes received: ", len);
  PRINTLN("Char: ", myData.a);
  PRINTLN("Int: ", myData.b);
  PRINTLN("Float: ", myData.c);
  PRINTLN("Bool: ", myData.d);
  PRINTLF;
}

MqttNowNode::MqttNowNode() : MqttNowBase() {};

void MqttNowNode::begin() {
  MqttNowBase::begin();
  WiFi.mode(WIFI_STA);
  PRINTLN("MAC address: ", WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    PRINTLNS("Error initializing ESP-NOW");
    return;
  }

  PRINTLNS("ESP-NOW initialized");

  esp_now_register_send_cb(OnDataSent);

  esp_now_register_recv_cb(onDataReceiver);

}

void MqttNowNode::update() {
  MqttNowBase::update();
}

esp_err_t MqttNowNode::addPeer(esp_now_peer_info_t *peer) {
  #ifdef ESP32
    if (!esp_now_is_peer_exist(peer->peer_addr)) {
      return esp_now_add_peer(peer);
    } else {
      return ESP_OK;
    }
  #elif defined(ESP8266)
    return addPeer(peer->peer_addr, peer->channel, peer->encrypt);
  #endif 
}

esp_err_t MqttNowNode::addPeer(uint8_t *mac_addr, uint8_t channel, bool encrypt) {
  #ifdef ESP32
    esp_now_peer_info_t info;
    memcpy(info.peer_addr, mac_addr, 6);
    info.channel = channel;
    info.encrypt = encrypt;
    if (encrypt == true) {
      memcpy(info.lmk, _lmk, ESP_NOW_KEY_LEN);
    }
    return addPeer(&info);
  #elif defined(ESP8266)
    // ESP 8266 has a simple method with std arguments to add a peer
    if (encrypt) {
      return esp_now_add_peer(mac_addr, ESP_NOW_ROLE_SLAVE, channel, _lmk, 16);
    } else {
      return esp_now_add_peer(mac_addr, ESP_NOW_ROLE_SLAVE, channel, NULL, 0);
    }
  #endif
}

#endif // !defined(MQTT_NOW_CLIENT)