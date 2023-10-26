# Serial communication

The communication between the MQTT client and the controller is a simple serial communication, using either the
hardware TX/RX connections or soft serial. This can be configured using the `-D COM=...` build flag. If this buildflag
is not defined it will default to `Serial`. This is defined in `mqtt-now-controller.h` and `mqtt-now-client.h`.

All serial messages are defined in `mqtt-now-bridge.h`.

### Prefix

`MSG_START "###"`

All serial messages have to start with a MSG_START prefix. This way the controller or client node know that it is
a valid message. If it does not start with a MSG_START prefix the message will be regarded as noise (or debug message)
and will therefore be ignored.

### Subscribe

`MSG_ACTIONSUB 'S'`

Following the MSG_START prefix, the action is defined.
