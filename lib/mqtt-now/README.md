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

| Board               | Environment                | Chip      | Controller         | MQTTClient/Webint  | Display            |
|---------------------|----------------------------|-----------|--------------------|--------------------|--------------------|
| ~~ESP-01 512K~~     | ~~esp01_512k~~ (*)         | ESP 8266  |                    |                    |                    |
| ~~ESP-01 512K~~     | ~~esp01_512k_client~~ (*)  | ESP 8266  |                    | :heavy_check_mark: |                    |
| ESP-01 1MB          | esp01_1m                   | ESP 8266  |                    |                    |                    |
| ESP-01 1MB          | esp01_1m_client            | ESP 8266  |                    | :heavy_check_mark: |                    |
| Wemos D1 mini       | d1_mini_lite               | ESP 8266  | :heavy_check_mark: |                    |                    |
| Heltec WifiKit 8    | heltec_wifi_kit_8          | ESP 8266  |                    |                    | :heavy_check_mark: |
| Wemos D1 mini32     | wemos_d1_mini32            | ESP 32    |                    |                    |                    |
| Wemos D1 mini32     | wemos_d1_mini32_control    | ESP 32    | :heavy_check_mark: |                    |                    |
| Do-It Devkit 32     | esp32doit-devkit-v1        | ESP 32    | :heavy_check_mark: |                    |                    |
| M5-Stack Fire       | m5stack-fire               | ESP 32    |                    |                    | :heavy_check_mark: |
| M5-Stack Fire       | m5stack-fire-control       | ESP 32    | :heavy_check_mark: |                    | :heavy_check_mark: |
| M5-Stack Fire       | m5stack-fire-client        | ESP 32    |                    | :heavy_check_mark: | :heavy_check_mark: |
| TenStar ESP32 mini  | esp32c3-mini               | ESP 32-C3 |                    |                    |                    |
| TenStar ESP32 mini  | esp32c3-mini_client        | ESP 32-C3 |                    | :heavy_check_mark: |                    |
| TenStar ESP32 mini  | esp32c3-mini_control       | ESP 32-C3 | :heavy_check_mark: |                    |                    |

(*) ESP-01 512K is no longer supported... Simply ain't gonna cut it.

---  

&nbsp;
&nbsp;

## Table Of Contents

[TOC]

**[Topology](#topology)**

**[Network ID](#network-id)**

**[Node Category](#node-category)**

**[Slave](#slave)**

**[Controller](#controller)**

**[Dedicated controller](#dedicated-controller)**

**[MQTT client](#mqtt-client)**

**[Protocol](#protocol)**

**[Controller selection](#controller-selection)**

**[MQTT](#mqtt)**

*[Root topic](#root-topic)*

*[Discovery topic](#discovery-topic)*

*[Command topic](#command-topic)*

*[Status topic](#status-topic)*

*[Node Command topic](#node-command-topic)*

*[Messages (ESP-Now)](#messages-esp-now)**

*[Introduction message](#introduction-message)*

*[Welcome message](#welcome-message)*

*[Request config message](#request-config-message)*

*[Configuration message](#configuration-message)*

*[Data message](#data-message)*

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

The non-controller nodes (aka [slaves](#slave)) communicate with the controller using the esp-now protocol. They report their status and listen for commands. They can also report events.

The controller is responsible for communication to the outside world over standard WiFi. Therefore the controller must be connected to an accesspoint or router connected to the home network and maybe the internet. The controller normally
is also playing a slave role.

Which device is the controller doesn't matter, since there is no hardware difference. All nodes are ESP32/ESP8266 based, and therefore they are all capable of communicating with a wireless network. The controller role is dynamically assigned (see [Controller selection](#controller-selection)) among the available nodes, and as soon the node player the controller role is offline, a new controller is chosen.

### Network ID

The Network ID is an UUID of 16 bytes. This ID is used to make sure no 'foreign' nodes can connect to this network. Currently this ID is set compile-time, so once a node is flashed, it can only connect to a certain network.
Also, this ID is sent each time a node boots up if not connected to a network to see if a network for it's ID is alive. Others couls pretend to be a controller, steal the ID and use it to connect to your home network. So this is not a safe algorithm yet, but the basic is there and it is now possible to run multiple MQTT-Now networks alongside eachother without any issues.
In the future there has to be some kind of encryption, with MAC address for example to make the ID unique for each node, but recognisable for the controller.

### Node category

There are several node categories. These categories describe in a nutshell what a node can and or does. They are based on the device types as defined in the [Home Assistant MQTT Discovery](https://www.home-assistant.io/docs/mqtt/discovery) documentation. This category can then be used by third party software to determine how to categorize the new node.

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

When a slave is turned on, it will send an [introduction message](#introduction-message) to the broadcast (MAC)address `FF:FF:FF:FF:FF:FF`. The introduction message contains the MAC address of the slave and name for easy recognition. It also contains a unique ID of the network it wants to connect to. For now this ID is hard coded, so it will have to be included at compile time for all nodes intended for the same network. The running controller will check the network ID, and if it matches its own network id it will respond with a [welcome message](#welcome-message) containing its mac address and register the slave as new node. The slave now knows who the controller is and will from now on address all communication to the controller directly using its MAC address. If there is no response from a running controller node, the new node will take on the role of controller (see [Controller initialization](#controller-initialization)) by connecting to the wifi network and wait for other nodes to come online as slaves.

The slave can be anything from a simple lightbulb switch, a complete weatherstation, smart plug or anything else you have in your smart-home. It will listen to esp-now messages and/or send messages.

## Controller

The controller is the central accesspoint to the whole MQTTNow network. Together with the [MQTT client](#mqtt-client) it acts as a gateway between the MQTTNow network and the home wifi, so that you can access these nodes with normal wifi enabled devices like a smartphone, computer or smart-home central like [Domoticz](https://www.domoticz.com/) or [Home Assistant](https://www.home-assistant.io).

Using this WiFi connection the controller will also connect if available to an [MQTT broker](#mqtt) for mqtt communication.

### Dedicated controller

### MQTT client

## Protocol

The protocol used for communication between the nodes is based on [esp-now](https://www.espressif.com/en/products/software/esp-now/overview). A low-power communication protocol using the same 2.4 Ghz radio that is used for wifi, but without the need for a central router. The nodes can communicate with eachother peer-to-peer.  
Since the nodes are not connected to the home network, there is no way to access them directly. Therefore the controller connects to the MQTTNow network, as well as to the home WiFi network and acts as a gateway. This way, only one IP address on the home network is needed for the whole esp=now network. For the outside world (everything not in the MQTTNow network) the network is available as one device.

### Controller selection

When a node (with controller capability) is turned on, and there is no controller present (eg, there is no [welcome message](#welcome-message) as a response to the [introduction message](#introduction-message)), this node shall become the controller. This is the most simple and most common scenario.

If in an existing network the active controller goes down, another will take over assuming that there is another one with controller capability. If there are multiple candidates, the one with the strongest wifi signal is selected.

### Controller initialization

## MQTT

The MQTT communication with [Home Assistant](https://www.home-assistant.io) (or any other application communicating with this network) goes over a couple of topics. These topics are subtopics of one [root topic](#root-topic).

    Root -+- \CMD -+-
          |        |--- node1
          |        |--- node2
          |
          |--- \stat -+--- \LWT
          |           |--- \state
          |           |--- node1 -+--- \LWT
          |           |           |--- \state
          |           |--- node2 -+--- \LWT
          |                       |--- \state
          |
          |--- \discovery -+-
                           |--- node1
                           |--- node2

All topics should have the `Retained` flag set to make sure a node can return to the last known correct state after reconnect or power cycle.

### Root topic

The root topic is where all other communication is organized. The root topic itself contains no payload, only subtopics.
General subtopics are a [command](#command-topic) topic and a [status](#status-topic) topic.
`LWT`  information is contained in the status topic, and shows wether or not the whole network is online. In other words, if the controller is connected to the MQTT broker. It can be either `online` or `offline`.
A third mandatory (sub)topic is the [Discovery topic](#discovery-topic). This topic holds all information needed for any client to identify all nodes and their abilities. It can also be used to add a node to a client automatically if it joins an MQTTNow network.

### Discovery topic

`\discovery`

The discovery topic can be used to automatically add nodes to 3rd party controlling software like [Home Assistant](https://www.home-assistant.io). Since the main idea of this project is to create a new protocol to be used by Home Assistant please refer to [HASS MQTT Discovery](https://www.home-assistant.io/docs/mqtt/discovery) for the exact registration payload. This (sub)topic can be configured without the prepending slash (/). This is usefull if the client has his own discovery topic, like Home Asisstant. In that case the discovery topic should be configured like `homeassistant`.

### Command topic

`\CMD`

The command topic can be used to send commands to the network as a whole. This can be something like ... This is typically set by 3rd party software. This topic also contains a subtopic for each node, identified by the node-id (see [Node Command Topic](#node-command-topic)).  
The controller subscribes to this topic to receive commands for either the whole network or for a specific node.

### Status topic

`\stat`

The Status topic is used by the controller to publish network status. It shows its online status in the `\LWT` subtopic and other information like controller name, network status information, IP address etc. For each node (and meaybe even
for the controller itself if it also has a node role) there is a [subtopic](#node-status-topic) containing status information for that node.

### Node Command topic

`\CMD\<node-id>`

Any device that wants to control a specific node, can publish a message to the node command topic. The full topic will be `root-topic\CMD\<node-id>`. What these commands can be is made available by the node.

### Node Status topic

`\stat\<node-id>`

The node status topic is used as feedback to the controller. The controller can see if the last command was successfully executed. Also, because it has the 'retained' flag set, the node can check what the last know status was in case of a powercycle or reconnect and go back to that state.

## Messages (ESP-Now)

ESP-Now supports messages with a maximum payload of 250 bytes. Therefore we have to be very careful with default meta data to make sure enough room is left for actual data. The only default metadata send with each message is the message type. Initial plan was to use a four character code to make it human readable, but that is replaced with a single one byte (`int8`) code. The Four character code is used as enumeration in the sourcecode.  
All `char[x]` fields **may** be shortened using a NULL termination. If no NULL byte occurs, the full length is used.

### Introduction message

(`intr - 1`)

The Introdction message is send when a node is powered up. It kinda says "Hello everybody. My name is John, this is what I do and who is the boss?". The message contains the following data:

| name | type | data | description |
|-|-|-|-|
| Message type | uint8 | 1 | Code for messagetype |
| MAC address | byte[6] | A1:B2:C3:D4:E5:F6 | MAC address if this node |
| Network UUID | byte[16] | 3d0bb7a9-f3e3-4fa5-86b7-b5f0aeca41ad | See [Network ID](#network-id) |
| Category | uint8 | 1 | See [Node category](#node-category) |
| Timeout | uint16 | 10 | Minutes before dead |
| Friendly name | char[30] | "light livingroom" | Descriptive name |

     t
     y                  C
     p         Network  a
     e  MAC      UUID   t  Friendly name
    |-|------|---~~~---|--|-----~~~-----|
     0  1-7    8-33     34   35-65 
    (total 66 bytes)

- **Message type**: Enumerated type of message being send. `1`
- **MAC Address**: MAC address of node sending this message. Used as unique identifier of the node.
- **Network UUID**: The unique ID of the network the node wants to connect to. Must be the same for all members of the network.
- **Category**: Identifies the kind of node and its capabilities. In HA this is used to determine the device type.
- **Timeout**: Tells the controller when to mark this node dead. If there is no communication coming from this node for x minutes, the node is considered dead.
- **Friendly name**: How to call this node in the human interface to easely identify it. For example 'light livingroom'.

### Welcome message

(`welc - 2`)

The Welcome message is the response of the controller to an [Introduction message](#introduction-message). Is basically says "Hi John, welcome. I'll write you down as a new member if this network, and from now on you only talk to me."

| name | type | data | description |
|-|-|-|-|
| Message type | int8 | 2 | Code for messagetype |
| MAC address | byte[6] | A1:B2:C3:D4:E5:F6 | Mac address of the controller |

     t
     y        
     p        
     e  MAC   
    |-|------|
     0  1-7   
    (total 8 bytes)

### Request config message

(`rqcf - 3`)

When a new node has become a member of a network (eg, the controller has responded to the
[Introduction Message](#introduction-message) with a [Welcome message](#welcome-message)), the node can ask for
configuration data. This data is needed if this node wants to be a backup controller node (see
[Controller Selection](#controller-selection)) in case the current controller goes down or loses wifi connectivity.

| name | type | data | description |
|-|-|-|-|
| Message type | int8 | 3 | Code for messagetype |

     t
     y        
     p        
     e  
    |-|
     0 
    (total 1 byte)

### Configuration message

(`cnfg - 4`)

The response to the [Request Config Message](#request-config-message). This message contains all data that is needed to
take over the role of controller. Being wifi and mqtt credentials.

| name | type | data | description |
|-|-|-|-|
| Message type | int8 | 4 | Code for messagetype |
| WiFi SSID | char[32] | My-wifi | SSID of your network |
| WiFi Key | char[64] | ***** | Authorization key for network |
| IP MQTT broker| byte[6] | A1:B2:C3:D4:E5:F6 | IP address of MQTT broker |
| MQTT port | int16 | 1883 | Port of MQTT broker |
| MQTT user | char[20] | user | MQTT user (blank if none) |
| MQTT password | char[20] | ***** | MQTT password (blank if none) |

     t                           P
     y                           o
     p                           r   MQTT    MQTT
     e  SSID   Key     IP MQTT   t   user    pwd
    |-|------|--~~~--|---------|---|--~~~--|--~~~--|
     0  1-32   33-96   97-102   103 105-125 126-146
                                 -
                                104
    (total 147 bytes)

### Data message

(`data - 5`)

| name | type | data | description |
|-|-|-|-|
| Message type | int8 | 5 | Code for messagetype |
| Data type | int8 | 1 | Code to describe type of data |
| Data | char[240] | whatever | The data itself |

     M D
     t t         
     y y         
     p p         
     e e  Data   
    |-|-|--~~~--|
     0 1  2-241 
                                
    (total 242 bytes)

### Error message

(`eror - 6`)

Is send as a response if an error occurs as a result of a request. For example when a node tries to join a network with an invalid network uuid.

| name | type | data | description |
|-|-|-|-|
| Message type | int8 | 5 | Code for messagetype |
| Error code | int8 | 1 | Error code  |
| Error message | char[240] | whatever | The error message |

&nbsp;
&nbsp;

## Serial communication

The communication between the MQTT client and the controller is a simple serial communication, using either the hardware TX/RX connections or soft serial. This can be configured using the `-D COM=...` build flag. If this buildflag is not defined it will default to `Serial`. This is defined in `mqtt-now-controller.h` and `mqtt-now-client.h`.

All serial messages are defined in `mqtt-now-bridge.h`.

### Prefix

`MSG_START "###"`

All serial messages have to start with a MSG_START prefix. This way the controller or client node know that it is a valid message. If it does not start with a MSG_START prefix the message will be regarded as noise (or debug message) and will therefore be ignored.

### Subscribe

`MSG_ACTIONSUB 'S'`

Following the MSG_START prefix, the action is defined.

## Library usage

### Includes

`mqtt-now.h`

### Defines

See `secrets.h`.

#### Build flags

If you define a new board for a specific situation, you can use the following build flags to customize the firmware:

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

This flag indicates the serial port that is to be used for serial communication between [controller](#controller) and [client](#mqtt-client) nodes.

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

## Wishlist

- Dynamic esp-now channel
- Web interface for gateway incl OTA
- Gateway as HASS device with entities for management and monitoring
- OTA for nodes
- Safer Network ID validation algorithm
