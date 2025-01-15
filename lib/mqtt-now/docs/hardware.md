## Hardware

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
