#ifndef __TENSTAR_ESP32_C3
#define __TENSTAR_ESP32_C3
#ifdef TENSTAR_ESP32C3
// To be used with esp32-c3-devkitm-1 board definition

// Add following buildflags to environment definition in platformio.ini to enable 
// serial output over USB:
// -D ARDUINO_USB_MODE=1
// -D ARDUINO_USB_CDC_ON_BOOT=1

#undef LED_BUILTIN
#undef BUILTIN_LED
#undef RGB_BUILTIN

#define LED_BUILTIN 8
#define BUILTIN_LED  LED_BUILTIN



#endif
#endif