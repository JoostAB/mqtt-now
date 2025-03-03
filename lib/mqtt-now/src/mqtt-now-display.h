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


#ifndef _M5STACK_H_

#define LCD

#ifndef WHITE
#define BLACK               0x0000      /*   0,   0,   0 */
#define NAVY                0x000F      /*   0,   0, 128 */
#define DARKGREEN           0x03E0      /*   0, 128,   0 */
#define DARKCYAN            0x03EF      /*   0, 128, 128 */
#define MAROON              0x7800      /* 128,   0,   0 */
#define PURPLE              0x780F      /* 128,   0, 128 */
#define OLIVE               0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY           0xC618      /* 192, 192, 192 */
#define DARKGREY            0x7BEF      /* 128, 128, 128 */
#define BLUE                0x001F      /*   0,   0, 255 */
#define GREEN               0x07E0      /*   0, 255,   0 */
#define CYAN                0x07FF      /*   0, 255, 255 */
#define RED                 0xF800      /* 255,   0,   0 */
#define MAGENTA             0xF81F      /* 255,   0, 255 */
#define YELLOW              0xFFE0      /* 255, 255,   0 */
#define WHITE               0xFFFF      /* 255, 255, 255 */
#define ORANGE              0xFD20      /* 255, 165,   0 */
#define GREENYELLOW         0xAFE5      /* 173, 255,  47 */
#define PINK                0xF81F
#endif

#else // _M5STACK_H_ defined
#include <mqtt-now-display-m5.h>
#endif


void setupDisplay();

void updateDisplay();

void log2Display(const char* txt);

#endif // HAS_DISPLAY
#endif // __MQTT_NOW_DISPLAY_H__
