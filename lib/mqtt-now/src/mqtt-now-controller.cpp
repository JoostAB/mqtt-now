#include <mqtt-now-controller.h>
#ifdef MQTT_NOW_CONTROLLER

MqttNowController::MqttNowController() : MqttNowNode() {};

void MqttNowController::begin() {
  MqttNowNode::begin();
};
void MqttNowController::update() {
  MqttNowNode::update();
};

#endif // MQTT_NOW_CONTROLLER