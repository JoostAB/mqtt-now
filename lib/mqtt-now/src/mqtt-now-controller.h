/**
 * @file mqtt-now-controller.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-02-28
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#if defined(MQTT_NOW_CONTROLLER) | defined(MQTT_TEST_COMPILE)
#ifndef __MQTT_NOW_CONTROLLER_H__
#define __MQTT_NOW_CONTROLLER_H__

#include <baseinclude.h>
#include <mqtt-now-node.h>
#include <mqtt-now-bridge.h>
#include <list>

#if !defined(COM)
#define COM Serial
#endif

using namespace std;

/**
 * @brief Controller of the MqttNow network. 
 * 
 * Relays all messages between the MqttNowClient and the MqttNow Node's in the esp-now network.
 * 
 */
class MqttNowController : public MqttNowNode {
  public:

    /**
     * @brief Creates a new MqttNowController instance
     */
    MqttNowController();

    
    /**
     * @brief Is triggered whenever a message is received over the esp-now network from an MqttNowNode.
     * 
     * @param macFrom Origin MAC address
     * @param type Type of message (one of msgType)
     * @param msg 
     * @param len 
     */
    void messageReceived(const uint8_t *macFrom, msgType type, msg_base *msg, uint8_t len);

    /**
     * @brief To be called once at startup of system
     */
    void begin();

    /**
     * @brief To be called as often as possible from system loop
     */
    void update(); 
  
  protected:
    result_t _doAction(char act);
  
  private:
    result_t _handleReceived();
    result_t _addNode(msg_intro* msg);
    /**
     * @brief A list of registered esp-now clients
     */
    std::list<esp_now_peer_info_t> _slaves;
};

#endif // __MQTT_NOW_CONTROLLER_H__
#endif // MQTT_NOW_CONTROLLER