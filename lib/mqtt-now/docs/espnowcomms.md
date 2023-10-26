# ESP-Now - Messages

ESP-Now supports messages with a maximum payload of 250 bytes. Therefore we have to be very careful with default meta
data to make sure enough room is left for actual data. The only default metadata send with each message is the message
type. Initial plan was to use a four character code to make it human readable, but that is replaced with a single one
byte (`int8`) code. The Four character code is used as enumeration in the sourcecode.  
All `char[x]` fields **may** be shortened using a NULL termination. If no NULL byte occurs, the full length is used.

### Introduction message

(`intr - 1`)

The Introdction message is send when a node is powered up. It kinda says "Hello everybody. My name is John, this is what
I do and who is the boss?". The message contains the following data:

| name | type | data | description |
|-|-|-|-|
| Message type | uint8 | 1 | Code for messagetype |
| MAC address | byte[6] | A1:B2:C3:D4:E5:F6 | MAC address if this node |
| Network UUID | byte[16] | 3d0bb7a9-f3e3-4fa5-86b7-b5f0aeca41ad | See [Network ID](../README.md#network-id) |
| Category | uint8 | 1 | See [Node category](../README.md#node-category) |
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
- **Network UUID**: The unique ID of the network the node wants to connect to. Must be the same for all members of the
network.
- **Category**: Identifies the kind of node and its capabilities. In HA this is used to determine the device type.
- **Timeout**: Tells the controller when to mark this node dead. If there is no communication coming from this node for
x minutes, the node is considered dead.
- **Friendly name**: How to call this node in the human interface to easely identify it. For example 'light livingroom'.

### Welcome message

(`welc - 2`)

The Welcome message is the response of the controller to an [Introduction message](#introduction-message). Is basically
says "Hi John, welcome. I'll write you down as a new member if this network, and from now on you only talk to me."

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
[Controller Selection](../README.md#controller-selection)) in case the current controller goes down or loses wifi connectivity.

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

Is send as a response if an error occurs as a result of a request. For example when a node tries to join a network with
an invalid network uuid.

| name | type | data | description |
|-|-|-|-|
| Message type | int8 | 5 | Code for messagetype |
| Error code | int8 | 1 | Error code  |
| Error message | char[240] | whatever | The error message |
