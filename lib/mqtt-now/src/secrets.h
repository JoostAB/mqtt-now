/**
 * @file secrets.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-05-16
 * 
 * @copyright Copyright (c) 2022
 * 
 * Fill in your own values, but make sure not to commit those changes!
 * use following command to ignore new changes
 * git update-index --assume-unchanged lib\mqtt-now\src\secrets.h 
 */
# pragma once
#ifndef __SECRETS_H__
#define __SECRETS_H__

#define MQTT_HOST "10.0.1.1"
#define MQTT_PORT 1883
#define MQTT_ID "mqtt_now"
#define MQTT_USER "mqtt_user"
#define MQTT_PW "mqtt_pw"

#define WIFI_SSID "myssid_name"
#define WIFI_PW "myssid_pw"

#ifndef __UUID_SET__
#define __UUID_SET__ 1
const uint8_t network_uuid[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
#endif

#endif // __SECRETS_H__
