# Wifi controlled Bot

Wifi Controlled bot that can be controlled using buttons on a mobile/desktop device.

## Table of Contents

- [Wifi controlled Bot](#wifi-controlled-bot)
  - [Table of Contents](#table-of-contents)
  - [Demonstration](#demonstration)
  - [About the Project](#about-the-project)
    - [Tech Stack](#tech-stack)
    - [File Structure](#file-structure)
    - [Data Processing](#data-processing)
      - [Supported Commands](#supported-commands)
    - [How to use](#how-to-use)
      - [Prerequisites](#prerequisites)
      - [Installation](#installation)
  - [Contributors](#contributors)
  - [Acknowledgements and Resources](#acknowledgements-and-resources)

## Demonstration

## About the Project

### Tech Stack

This project uses the [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/) development framework.

It uses [FreeRTOS](https://www.freertos.org/openrtos.html).

### File Structure

```
.
├── CMakeLists.txt
├── Makefile
├── README.md
├── components
│   ├── sra-board-component
│   └── websocket
├── frontend
│   └── index.html
├── main
│   ├── CMakeLists.txt
│   ├── Kconfig.projbuild
│   ├── comms_websocket_server.c
│   ├── component.mk
│   ├── include
│   ├── main.c
│   └── wifi_handler.c
├── sdkconfig
└── sdkconfig.old
```

### Data Processing

Commands are sent to the websocket on the bot attached to a websocket packet payload.


Payload format:
```
<command prefix><command parameter>
```

#### Supported Commands

For direction:

- `R` for right.
- `L` for left.
- `F` for forward.
- `B` for backwards.
- `T` for test ping.

For speed:

`S<speed%>`

### How to use

#### Prerequisites

Install ESP-IDF : https://github.com/espressif/esp-idf

#### Installation

Clone the project

```sh
git clone https://github.com/SuperChamp234/wifi-bot-sra
cd wifi-bot-sra
```

Building the project

```sh
idf.py build
```

Flash

```sh
idf.py -p (PORT) flash monitor
```

and before disconnecting, configuring the wifi

```sh
idf.py menuconfig
```

Under the `Wall-E Bot Configuration`

- `WIFI SSID` - Set the Wifi SSID
- `WIFI PASSWORD` - Set wifi Password

---

## Contributors

- [Zain Siddavatam](https://github.com/SuperChamp234/RISC)


## Acknowledgements and Resources
- [SRA VJTI](https://github.com/SRA-VJTI)
- [SRA Wall-E Workshop](https://github.com/SRA-VJTI/Wall-E_v2.2)
- Special thanks to [Moteen Shah](https://github.com/Jamm02)
- https://github.com/espressif/esp-idf/tree/release/v4.2/examples/protocols/http_server
- https://github.com/Molorius/esp32-websocket/
