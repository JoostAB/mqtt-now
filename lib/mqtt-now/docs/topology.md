## Topology

[TOC]

A network consists of several nodes, from which one has the role of [controller](controller.md).

The non-controller nodes (aka [slaves](slave.md)) communicate with the controller using the esp-now protocol. They report their status and listen for commands. They can also report events.

The controller is responsible for communication to the outside world over standard WiFi. Therefore the controller must be connected to an accesspoint or router connected to the home network and maybe the internet. The controller normally
is also playing a slave role.

Which device is the controller doesn't matter, since there is no hardware difference. All nodes are ESP32/ESP8266 based, and therefore they are all capable of communicating with a wireless network. The controller role is dynamically assigned (see [Controller selection](controller.md) ) among the available nodes, and as soon the node player the controller role is offline, a new controller is chosen.

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
