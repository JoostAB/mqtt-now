## MQTT

[TOC]

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
