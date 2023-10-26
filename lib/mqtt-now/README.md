# MQTTNow

## Overview

An implementation of the ESP-Now protocol in combination with MQTT.

The idea is to use this protocol for several IOT devices in my smart home setup.
One node will be the bridge to my [Home Assistant](https://www.home-assistant.io) using MQTT. All others communicate to
this bridge using ESP-Now.
The advantage is that there is only one node connected to my WiFi network, reducing used IP addresses and workload for
router and Access points.

Since I use all different kinds of hardware for my devices (different kinds of ESP32/ESP8266 boards), the code has to
be so portable as possible.
Different build definitions are included in the platformio*.ini files.

The following boards have been defined:

| Board               | Environment                | Chip     | Controller         | MQTTClient/Webint  | Display            |
|---------------------|----------------------------|----------|--------------------|--------------------|--------------------|
| ~~ESP-01 512K~~     | ~~esp01_512k~~ (*)         | ESP 8266 |                    |                    |                    |
| ~~ESP-01 512K~~     | ~~esp01_512k_client~~ (*)  | ESP 8266 |                    | :heavy_check_mark: |                    |
| ESP-01 1MB          | esp01_1m                   | ESP 8266 |                    |                    |                    |
| ESP-01 1MB          | esp01_1m_client            | ESP 8266 |                    | :heavy_check_mark: |                    |
| Wemos D1 mini       | d1_mini_lite               | ESP 8266 | :heavy_check_mark: |                    |                    |
| Heltec WifiKit 8    | heltec_wifi_kit_8          | ESP 8266 |                    |                    | :heavy_check_mark: |
| Wemos D1 mini32     | wemos_d1_mini32            | ESP 32   |                    |                    |                    |
| Wemos D1 mini32     | wemos_d1_mini32_control    | ESP 32   | :heavy_check_mark: |                    |                    |
| Do-It Devkit 32     | esp32doit-devkit-v1        | ESP 32   | :heavy_check_mark: |                    |                    |
| M5-Stack Fire       | m5stack-fire               | ESP 32   |                    |                    | :heavy_check_mark: |
| M5-Stack Fire       | m5stack-fire-control       | ESP 32   | :heavy_check_mark: |                    | :heavy_check_mark: |
| M5-Stack Fire       | m5stack-fire-client        | ESP 32   |                    | :heavy_check_mark: | :heavy_check_mark: |

(*) ESP-01 512K is no longer supported... Simply ain't gonna cut it.

---  

&nbsp;
&nbsp;

## Table Of Contents

**[Topology](#topology)**

**[Network ID](#network-id)**

**[Node Category](#node-category)**

**[Slave](#slave)**

**[Controller](#controller)**

**[Dedicated controller](#dedicated-controller)**

**[MQTT client](#mqtt-client)**

**[Protocol](#protocol)**

**[Controller selection](#controller-selection)**

**[MQTT](docs/mqtt.md)**

**[ESP-Now messages](docs/espnowcomms.md)**

**[Serial communication](docs/serialcomms.md)**

**[Library usage](#library-usage)**

*[Includes](#includes)*

*[Defines](#defines)*

*[Build Flags](#build-flags)*

*[Examples](#examples)*

*[Compilation](#compilation)*

&nbsp;
&nbsp;

## Topology

A network consists of several nodes, from which one has the role of [controller](#controller).

The non-controller nodes (aka [slaves](#slave)) communicate with the controller using the esp-now protocol. They report
their status and listen for commands. They can also report events.

The controller is responsible for communication to the outside world over standard WiFi. Therefore the controller must
be connected to an accesspoint or router connected to the home network and maybe the internet. The controller normally
is also playing a slave role.

Which device is the controller doesn't matter, since there is no hardware difference. All nodes are ESP32/ESP8266 based,
and therefore they are all capable of communicating with a wireless network. The controller role is dynamically assigned
(see [Controller selection](#controller-selection)) among the available nodes, and as soon the node player the
controller role is offline, a new controller is chosen.

### Network ID

The Network ID is an UUID of 16 bytes. This ID is used to make sure no 'foreign' nodes can connect to this network.
Currently this ID is set compile-time, so once a node is flashed, it can only connect to a certain network.
Also, this ID is sent each time a node boots up if not connected to a network to see if a network for it's ID is alive.
Others could pretend to be a controller, steal the ID and use it to connect to your home network. So this is not a safe
algorithm yet, but the basic is there and it is now possible to run multiple MQTT-Now networks alongside eachother
without any issues.
In the future there has to be some kind of encryption, with MAC address for example to make the ID unique for each node,
but recognisable for the controller.

### Node category

There are several node categories. These categories describe in a nutshell what a node can and or does. They are based
on the device types as defined in the [Home Assistant MQTT Discovery](https://www.home-assistant.io/docs/mqtt/discovery)
documentation. This category can then be used by third party software to determine how to categorize the new node.

Available categories are:

- alarmControlPanel (*)
- binarySensor (*)
- button (*)
- camera (*)
- cover (*)
- deviceTracker (*)
- deviceTrigger (*)
- fan (*)
- humidifier (*)
- hvac (*)
- light (*)
- lock (*)
- number (*)
- scene (*)
- select (*)
- sensor (*)
- siren (*)
- switch (*)
- tagScanner (*)
- vacuum (*)

<sub>(*) = Not implemented yet</sub>

## Slave

When a slave is turned on, it will send an [introduction message](docs/espnowcomms.md#introduction-message) to the
broadcast (MAC)address `FF:FF:FF:FF:FF:FF`. The introduction message contains the MAC address of the slave and name for
easy recognition. It also contains a unique ID of the network it wants to connect to. For now this ID is hard coded, so
it will have to be included at compile time for all nodes intended for the same network. The running controller will
check the network ID, and if it matches its own network id it will respond with a [welcome message](docs/espnowcomms.md#welcome-message)
containing its mac address and register the slave as new node. The slave now knows who the controller is and will from
now on address all communication to the controller directly using its MAC address. If there is no response from a
running controller node, the new node will take on the role of controller (see
[Controller initialization](#controller-initialization)) by connecting to the wifi network and wait for other nodes to
come online as slaves.

The slave can be anything from a simple lightbulb switch, a complete weatherstation, smart plug or anything else you
have in your smart-home. It will listen to esp-now messages and/or send messages.

## Controller

The controller is the central accesspoint to the whole MQTTNow network. Together with the [MQTT client](#mqtt-client)
it acts as a gateway between the MQTTNow network and the home wifi, so that you can access these nodes with normal wifi
enabled devices like a smartphone, computer or smart-home central like [Domoticz](https://www.domoticz.com/) or
[Home Assistant](https://www.home-assistant.io).

Using this WiFi connection the controller will also connect if available to an
[MQTT broker](docs/mqtt.md#mqtt-structure) for mqtt communication.

### Dedicated controller

### MQTT client

## Protocol

The protocol used for communication between the nodes is based on
[esp-now](https://www.espressif.com/en/products/software/esp-now/overview). A low-power communication protocol using
the same 2.4 Ghz radio that is used for wifi, but without the need for a central router. The nodes can communicate with
eachother peer-to-peer.  
Since the nodes are not connected to the home network, there is no way to access them directly. Therefore the controller
connects to the MQTTNow network, as well as to the home WiFi network and acts as a gateway. This way, only one IP
address on the home network is needed for the whole esp=now network. For the outside world (everything not in the
MQTTNow network) the network is available as one device.

### Controller selection

When a node (with controller capability) is turned on, and there is no controller present (eg, there is no
[welcome message](docs/espnowcomms.md#welcome-message) as a response to the
[introduction message](docs/espnowcomms.md#introduction-message)), this node shall become the controller. This is the
most simple and most common scenario.

If in an existing network the active controller goes down, another will take over assuming that there is another one
with controller capability. If there are multiple candidates, the one with the strongest wifi signal is selected.

### Controller initialization

&nbsp;
&nbsp;

## Library usage

### Includes

`mqtt-now.h`

### Defines

See `secrets.h`.

#### Build flags

If you define a new board for a specific situation, you can use the following build flags to customize the firmware:

##### DEBUGLOG

`-D DEBUGLOG=1`

If set, the DEBUGLOG flag enabled debug logging to the serial interface. Beware that you cannot use this in productio if
you are running a client or controller firmware that uses the same serial port for communication. During development
this is usefull for debugging, but also to send/receive the communication send between controller and client.

##### MQTTNOW_DEBUG

`-D MQTTNOW_DEBUG=1`

Not implemented yet.

##### HAS_DISPLAY

`-D HAS_DISPLAY=1`

Indicates that the node has a display, and this includes the `mqtt-now-display.h` file. Currently the only
implementation is for M5Stack devices.

##### COM

`-D COM=Serial`

This flag indicates the serial port that is to be used for serial communication between [controller](#controller) and
[client](#mqtt-client) nodes.

##### MQTT_NOW_CLIENT

`-D MQTT_NOW_CLIENT=1`

Setting the MQTT_NOW_CLIENT flag compiles the client version of the software. This includes a wifi connection, web
server and MQTT client.

##### MQTT_NOW_CONTROLLER

`-D MQTT_NOW_CONTROLLER=1`

This flag compiles the controller node. Implements everything a normal node has but also the management logic and serial
communication with the client node.

If neither the `MQTT_NOW_CLIENT` or `MQTT_NOW_CONTROLLER` are set, a standard slave node will be created.

### Examples

None yet

### Compilation

Best to build this in a PIO environment, since all configuration is done.
In platformio.ini just select the envoironment you want to build, or extend one to add/modify [flags](#build-flags).

## Wishlist

- Dynamic esp-now channel
- Web interface for gateway incl OTA
- Gateway as HASS device with entities for management and monitoring
- OTA for nodes
- Safer Network ID validation algorithm
