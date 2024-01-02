/**
 * @file baseinclude.h
 * @author Joost Bloemsma (joost.a@bloemsma.net)
 * @brief 
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <Arduino.h>

#ifndef __BASE_INCLUDE_H__
#define __BASE_INCLUDE_H__

typedef int8_t result_t;

const result_t result_success = 1;
const result_t result_error = -1;

#include <jbdebug.h>
#include <secrets.h>

#ifdef ESP8266
  //#include <ESP8266WebServer.h>
  #include <ESP8266WiFi.h>
  // #include <ESPAsyncTCP.h>
  //typedef ESP8266WebServer WServer;
#elif defined(ESP32)
  #include <WiFi.h>
  // #include <WebServer.h>
  #include <AsyncTCP.h>
  //typedef WebServer WServer;
#endif


#ifdef M5STACK_FIRE
  #include <Wire.h> 
  #include <M5Stack.h> 
#endif

#ifdef HAS_DISPLAY
  #include <mqtt-now-display.h>
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

typedef AsyncWebServer WServer;

#if !defined(SERIALBAUDRATE)
#define SERIALBAUDRATE 115200
#endif

#define SIZE_IP 4
#define SIZE_MAC 6

#endif // __BASE_INCLUDE_H__