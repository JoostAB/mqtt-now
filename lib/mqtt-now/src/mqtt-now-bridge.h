/**
 * @file mqtt-now-bridge.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-04-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#if defined(MQTT_NOW_CLIENT) || defined(MQTT_NOW_CONTROLLER)
#ifndef __MQTT_NOW_BRIDGE__
#define __MQTT_NOW_BRIDGE__

// Serial Protocol definition
#define MSG_START "###"
#define MSG_PAYLOAD "#P#"
#define MSG_ACTIONSUB 'S'
#define MSG_ACTIONPUB 'P'

#endif // __MQTT_NOW_BRIDGE__
#endif // MQTT_NOW_CLIENT | MQTT_NOW_CONTROLLER