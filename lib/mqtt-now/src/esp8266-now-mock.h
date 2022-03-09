/**
 * @file esp8266-now-mock.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */
# pragma once
#ifndef __ESP8266_NOW_MOCK__
#define __ESP8266_NOW_MOCK__
// Following are defines from esp_now.h for ESP 32
// Defined here for compatibility
#ifdef ESP8266
#include <espnow.h>

// Following are defines from esp_now.h for ESP 32
// Defined here for compatibility
typedef int32_t esp_err_t;

/* Definitions for error constants. */
#define ESP_OK                          0   /*!< esp_err_t value indicating success (no error) */
#define ESP_FAIL                       -1   /*!< Generic esp_err_t code indicating failure */
#define ESP_ERR_NO_MEM              0x101   /*!< Out of memory */
#define ESP_ERR_INVALID_ARG         0x102   /*!< Invalid argument */
#define ESP_ERR_INVALID_STATE       0x103   /*!< Invalid state */
#define ESP_ERR_INVALID_SIZE        0x104   /*!< Invalid size */
#define ESP_ERR_NOT_FOUND           0x105   /*!< Requested resource not found */
#define ESP_ERR_NOT_SUPPORTED       0x106   /*!< Operation or feature not supported */
#define ESP_ERR_TIMEOUT             0x107   /*!< Operation timed out */
#define ESP_ERR_INVALID_RESPONSE    0x108   /*!< Received response was invalid */
#define ESP_ERR_INVALID_CRC         0x109   /*!< CRC or checksum was invalid */
#define ESP_ERR_INVALID_VERSION     0x10A   /*!< Version was invalid */
#define ESP_ERR_INVALID_MAC         0x10B   /*!< MAC address was invalid */

#define ESP_NOW_ETH_ALEN             6         /*!< Length of ESPNOW peer MAC address */
#define ESP_NOW_KEY_LEN              16        /*!< Length of ESPNOW peer local master key */

#define ESP_NOW_MAX_TOTAL_PEER_NUM   20        /*!< Maximum number of ESPNOW total peers */
#define ESP_NOW_MAX_ENCRYPT_PEER_NUM 6         /*!< Maximum number of ESPNOW encrypted peers */

#define ESP_NOW_MAX_DATA_LEN         250       /*!< Maximum length of ESPNOW data which is sent very time */

typedef enum {
    ESP_NOW_SEND_SUCCESS = 0,       /* Send ESPNOW data successfully */
    ESP_NOW_SEND_FAIL,              /* Send ESPNOW data fail */
} esp_now_send_status_t;

typedef enum {
    ESP_IF_WIFI_STA = 0,     /**< ESP32 station interface */
    ESP_IF_WIFI_AP,          /**< ESP32 soft-AP interface */
    ESP_IF_ETH,              /**< ESP32 ethernet interface */
    ESP_IF_MAX
} wifi_interface_t;

typedef struct esp_now_peer_info {
  uint8_t peer_addr[ESP_NOW_ETH_ALEN];    /**< ESPNOW peer MAC address that is also the MAC address of station or softap */
  uint8_t lmk[ESP_NOW_KEY_LEN];           /**< ESPNOW peer local master key that is used to encrypt data */
  uint8_t channel;                        /**< Wi-Fi channel that peer uses to send/receive ESPNOW data. If the value is 0,
                                                use the current channel which station or softap is on. Otherwise, it must be
                                                set as the channel that station or softap is on. */
  wifi_interface_t ifidx;                 /**< Wi-Fi interface that peer uses to send/receive ESPNOW data */
  bool encrypt;                           /**< ESPNOW data that this peer sends/receives is encrypted or not */
  void *priv;                             /**< ESPNOW peer private data */
} esp_now_peer_info_t;

#endif
#endif // __ESP8266_NOW_MOCK__