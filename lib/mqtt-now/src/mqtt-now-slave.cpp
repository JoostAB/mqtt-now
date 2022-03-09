#include <mqtt-now-slave.h>
#if !(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))

MqttNowSlave::MqttNowSlave() {};

void MqttNowSlave::begin() {};
void MqttNowSlave::update() {};

#endif // !(defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER))