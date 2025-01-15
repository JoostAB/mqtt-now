## Library usage

[TOC]

### Includes

`mqtt-now.h`

### Defines

See `secrets.h`.

MQTT_HOST: IP or hostname of the MQTT broker
MQTT_PORT: Port of the MQTT broker (usually 1883 on Mosquitto)
MQTT_ID: Client ID to be used on the MQTT broker (Not the MQTT user)
MQTT_USER: The user to log in to the MQTT broker
MQTT_PW: The password to log in to the MQTT broker

WIFI_SSID: SSID of wifi network to connect to
WIFI_PW: Password of the wifi network

#### Build flags

If you define a new board for a specific situation, you can use the following build flags to customize the firmware:
(These flags are preset for the defined boards)

##### DEBUGLOG

`-D DEBUGLOG=1`

If set, the DEBUGLOG flag enabled debug logging to the serial interface. Beware that you cannot use this in productio if you are running a client or controller firmware that uses the same serial port for communication. During development this is usefull for debugging, but also to send/receive the communication send between controller and client.

##### MQTTNOW_DEBUG

`-D MQTTNOW_DEBUG=1`

Not implemented yet.

##### HAS_DISPLAY

`-D HAS_DISPLAY=1`

Indicates that the node has a display, and this includes the `mqtt-now-display.h` file. Currently the only implementation is for M5Stack devices.

##### COM

`-D COM=Serial`

This flag indicates the serial port that is to be used for serial communication between [controller](controller.md) and [client](mqtt.md#mqtt-client) nodes.

##### MQTT_NOW_CLIENT

`-D MQTT_NOW_CLIENT=1`

Setting the MQTT_NOW_CLIENT flag compiles the client version of the software. This includes a wifi connection, web server and MQTT client.

##### MQTT_NOW_CONTROLLER

`-D MQTT_NOW_CONTROLLER=1`

This flag compiles the controller node. Implements everything a normal node has but also the management logic and serial communication with the client node.

If neither the `MQTT_NOW_CLIENT` or `MQTT_NOW_CONTROLLER` are set, a standard slave node will be created.

### Examples

None yet

### Compilation

Best to build this in a PIO environment, since all configuration is done.
In platformio.ini just select the envoironment you want to build, or extend one to add/modify [flags](#build-flags).
