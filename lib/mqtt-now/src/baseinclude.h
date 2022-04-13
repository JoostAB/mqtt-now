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
#include <jbdebug.h>
#include <secrets.h>

#if !defined(SERIALBAUDRATE)
#define SERIALBAUDRATE 115200
#endif