/**
 * @file mqtt-now-display.h
 * @brief 
 * @author Joost Bloemsma (https://github.com/JoostAB)
 * @version 0.0.1
 * @date 2023-07-14
 */
# pragma once
#ifndef __MQTT_NOW_DISPLAY_H__
#define __MQTT_NOW_DISPLAY_H__

#include <baseinclude.h>

#ifdef HAS_DISPLAY
void setupDisplay();

void updateDisplay();

void log2Display(const char* txt);

#endif // HAS_DISPLAY
#endif // __MQTT_NOW_DISPLAY_H__
