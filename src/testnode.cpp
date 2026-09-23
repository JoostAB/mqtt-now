#include <testnode.h>

MqttNowTestNode::MqttNowTestNode() : MqttNowSlave() {};

void MqttNowTestNode::begin() {
  MqttNowSlave::begin();
};

void MqttNowTestNode::update() {
  MqttNowSlave::update();
  if (M5.BtnA.wasPressed()) {
    log2Display("Button A was pressed");
    const uint8_t brc[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    sendIntroMessage(1,"test",brc);
  }
  if (M5.BtnB.wasPressed()) {
    log2Display("Button B was pressed");
  }
  if (M5.BtnC.wasPressed()) {
    log2Display("Button C was pressed");
  }
};