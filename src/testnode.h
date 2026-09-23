#pragma once
#ifndef _MN_TESTNODE_H__
#define _MN_TESTNODE_H__ 1

#include <mqtt-now.h>

class MqttNowTestNode : public MqttNowSlave {
  public:
    MqttNowTestNode();
    //~MqttNowTestNode();
    void
      begin(),
      update();
};

#endif // _MN_TESTNODE_H__