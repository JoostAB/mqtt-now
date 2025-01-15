## Controller

[TOC]

The controller is the central accesspoint to the whole MQTTNow network. Together with the [MQTT client](#mqtt-client) it acts as a gateway between the MQTTNow network and the home wifi, so that you can access these nodes with normal wifi enabled devices like a smartphone, computer or smart-home central like [Domoticz](https://www.domoticz.com/) or [Home Assistant](https://www.home-assistant.io).

Using this WiFi connection the controller will also connect if available to an [MQTT broker](mqtt.md) for mqtt communication.

### Controller selection

When a node (with controller capability) is turned on, and there is no controller present (eg, there is no [welcome message](protocol.md#welcome-message) as a response to the [introduction message](lcome-#introduction-message)), this node shall become the controller. This is the most simple and most common scenario.

If in an existing network the active controller goes down, another will take over assuming that there is another one with controller capability. If there are multiple candidates, the one with the strongest wifi signal is selected.

### Controller initialization

### Dedicated controller

### MQTT client
