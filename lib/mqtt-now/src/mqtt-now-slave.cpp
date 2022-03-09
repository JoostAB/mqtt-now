#include <mqtt-now-slave.h>
#if !(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))

MqttNowSlave::MqttNowSlave() : MqttNowNode() {};

void MqttNowSlave::begin() {
  MqttNowNode::begin();
};
void MqttNowSlave::update() {
  MqttNowNode::update();
};

#endif // !(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))