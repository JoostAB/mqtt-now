## Overview {#mainpage}

An implementation of the ESP-Now protocol in combination with MQTT.

The idea is to use this protocol for several IOT devices in my smart home setup.
One node will be the bridge to my [Home Assistant](https://www.home-assistant.io) using MQTT. All others communicate to
this bridge using ESP-Now.
The advantage is that there is only one node connected to my WiFi network, reducing used IP addresses and workload for
router and Access points.
