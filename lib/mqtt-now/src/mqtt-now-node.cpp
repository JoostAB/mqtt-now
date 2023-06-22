/**
 * @file mqtt-now-node.cpp
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <mqtt-now-node.h>
#if (!defined(MQTT_NOW_CLIENT)) | defined(MQTT_TEST_COMPILE)

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

MqttNowNode *node;

struct_msg wrapper;

esp_now_peer_info_t peerInfo;

/*************************/
/** OnDataSent Callback **/
/*************************/

#if defined(ESP32)
  void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
#elif defined(ESP8266)
  void OnDataSent(uint8_t *mac_addr, uint8_t status) {
#endif
  #if DEBUGLOG
    PRINTS("Last Packet Send Status: ");
    if (status == ESP_NOW_SEND_SUCCESS) {
      PRINTLN("Delivery ", "success");
    } else {
      PRINTLN("Delivery ", "fail")
    }
  #endif
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
  memcpy(&wrapper, incomingData, len);
  #ifndef MQTT_NOW_CONTROLLER
    if (wrapper.type == msgTypeIntro || wrapper.type == msgTypeReqConfig) {
      // These messages are only intended for the controller, so ignore them
      return;
    }
  #else
    if (wrapper.type == msgTypeConfig || wrapper.type == msgTypeWelcome) {
      // These messages are NOT intended for the controller
      return;
    }
  #endif
  node->messageReceived(mac, wrapper.type, wrapper.contents, wrapper.msgSize);
}

MqttNowNode::MqttNowNode() : MqttNowBase() {};

void MqttNowNode::begin() {
  node = this;
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

// void MqttNowNode::messageReceived(const uint8_t *macFrom, uint8_t type, msg_base *msg, uint8_t len) {

// }

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

esp_err_t MqttNowNode::sendIntroMessage(uint8_t category, char friendlyName[30], const uint8_t *macReceiver) {
  msg_intro *msg;
  getMessageStruct(msgTypeIntro, msg);
  msg->device_category = category;
  memcpy(msg->friendly_name, friendlyName, 30);
  return sendMessage(msg, macReceiver);
}

esp_err_t MqttNowNode::sendWelcomeMessage(const uint8_t *macReceiver) {
  msg_welcome *msg;
  getMessageStruct(msgTypeWelcome, msg);
  getMac(msg->mac_address);
  return sendMessage(msg, macReceiver);
}

esp_err_t MqttNowNode::sendReqCfgMessage(const uint8_t *macReceiver) {
  msg_welcome *msg;
  getMessageStruct(msgTypeWelcome, msg);
  getMac(msg->mac_address);
  return sendMessage(msg, macReceiver);
}

esp_err_t MqttNowNode::sendErrorMessage(uint8_t error_code, const char error_msg[240], const uint8_t *macReceiver) {
  msg_error *msg;
  getMessageStruct(msgTypeError, msg);
  msg->error_code = error_code;
  strcpy(msg->error_msg, error_msg);
  return sendMessage(msg, macReceiver);
}

esp_err_t MqttNowNode::sendMessage(msg_base *msg, const uint8_t *macReceive) {
  struct_msg wrapper;
  wrapper.type = msg->msgtype;
  wrapper.msgSize = sizeof(&msg);
  memcpy(wrapper.contents, msg, wrapper.msgSize);
  return esp_now_send(macReceive, (uint8_t *) &wrapper, sizeof(wrapper));
}

esp_err_t MqttNowNode::sendMessage(msgType type, msg_base *msg, const uint8_t *macReceive) {
  struct_msg wrapper;
  
  // wrapper.contents will be initialized in getMessageStruct, so ignore warnings
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
  #pragma GCC diagnostic ignored "-Wuninitialized"
  if (getMessageStruct(type, wrapper.contents) == result_error) return ESP_ERR_INVALID_ARG;
  #pragma GCC diagnostic pop

  wrapper.type = type;
  wrapper.msgSize = sizeof(&msg);
  memcpy(wrapper.contents, msg, wrapper.msgSize);
  return esp_now_send(macReceive, (uint8_t *) &wrapper, sizeof(wrapper));
}

result_t MqttNowNode::getMessageStruct(msgType type, msg_base *msg) {
  switch (type) {
    case msgTypeIntro:
      msg = new msg_intro;
      break;
    case msgTypeWelcome:
      msg = new msg_welcome;
      break;
    case msgTypeReqConfig:
      msg = new msg_reqconfig;
      break;
    case msgTypeConfig:
      msg = new msg_config;
      break;
    case msgTypeData:
      msg = new msg_data;
      break;
    case msgTypeError:
      msg = new msg_error;
      break;
    default:
      return result_error;
  }
  msg->msgtype = type;
  return result_success;
}

#endif // !defined(MQTT_NOW_CLIENT)