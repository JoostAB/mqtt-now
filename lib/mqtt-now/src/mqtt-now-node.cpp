#include <mqtt-now-node.h>
#if !defined(MQTT_NOW_CLIENT)

MqttNowNode::MqttNowNode() {};
void MqttNowNode::begin() {};
void MqttNowNode::update() {};

int MqttNowNode::addpeer(uint8_t *mac_addr, uint8_t channel, bool encrypt) {
  #ifdef ESP32
      // ESP 32 uses a structure for peer information      
      memcpy(peerInfo.peer_addr, mac_addr, 6);
      peerInfo.channel = channel;
      peerInfo.encrypt = encrypt;
      if (encrypt == true) {
        memcpy(peerInfo.lmk, _lmk, ESP_NOW_KEY_LEN);
      }
      return esp_now_add_peer(&peerInfo);
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