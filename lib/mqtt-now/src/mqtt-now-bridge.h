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

#ifndef MQTT_ROOT_TOPIC
#define MQTT_ROOT_TOPIC "mqtt-now"
#endif

#ifndef MQTT_CMD_TOPIC
#define MQTT_CMD_TOPIC "cmd"
#endif

#ifndef MQTT_STATUS_TOPIC
#define MQTT_STATUS_TOPIC "status"
#endif

#ifndef MQTT_LWT_TOPIC
#define MQTT_LWT_TOPIC "lwt"
#endif

#ifndef MQTT_DEV_TOPIC
#define MQTT_DEV_TOPIC "devices"
#endif

#ifndef MQTT_ON_CMD
#define MQTT_ON_CMD "on"
#endif

#ifndef MQTT_OFF_CMD
#define MQTT_OFF_CMD "off"
#endif

#ifndef MQTT_ONLINE_LWT
#define MQTT_ONLINE_LWT "online"
#endif

#ifndef MQTT_OFFLINE_LWT
#define MQTT_OFFLINE_LWT "offline"
#endif

// The pathseperator for MQTT topic paths
#define PATHSEP '/'


/* Serial Protocol definition */

// All messages start with MSG_START tag, just to be sure noise gets filtered out
#define MSG_START "###"

// First character after MSG_START defines the action (type of message)
#define MSG_ACTIONSUB 'S' // Subscribe to following topic
                          // Directly after MSG_ACTIONSUB a 0,1 or 2 should follow to identify QOS

#define MSG_ACTIONUNS 'U' // Unsubscribe from following topic

#define MSG_ACTIONPUB 'P' // Publish payload to topic
                          // Directly after MSG_ACTIONPUB a 0,1 or 2 should follow to identify QOS,
                          // followed by 1 or 0 for retained flag

// In case of publish, topic and payload are seperated by MSG_PAYLOAD tag
#define MSG_PAYLOAD "#P#"

// Examples of messages:
// 
// ###S1my-topic/status           : Subscibe to topic 'my-topic/status' with QOS 1
// ###Umy-topic/status            : Unsubscibe from topic 'my-topic/status'
// ###P11my-topic/status#P#data   : Publish payload 'data' to topic 'my-topic/status' with QOS 1
//                                  and retained flag set to true
//
// Notes:
// - If a topic starts with '/', it is presumed to be a device specific message and the topic will
//   be prepended with 'MQTT_ROOT_TOPIC/MQTT_DEV_TOPIC/'
// - To access the MQTT_ROOT_TOPIC (eg 'mqtt-now' by default) start your topic with '@'

#endif // __MQTT_NOW_BRIDGE__
#endif // MQTT_NOW_CLIENT | MQTT_NOW_CONTROLLER