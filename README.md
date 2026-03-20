# zephyr-stts22h

A minimal Zephyr RTOS application that reads temperature data
from an **STTS22H** sensor over I2C.

## Prerequisites

- Zephyr SDK / Zephyr SDK toolchain (follow the [Getting Started](https://docs.zephyrproject.org/latest/develop/getting_started/index.html) guide)
- A supported board with an I2C bus
- STTS22H wired to the board's I2C bus (`SDA`/`SCL`) — on the **nRF9151 DK**
  these are pins **P0.30 (SDA)** and **P0.31 (SCL)** on the Arduino header
  — with: STTS22H at address `0x3C` by default

## Initialize the workspace

```bash
mkdir zephyr-stts22h-ws && cd zephyr-stts22h-ws
python3 -m venv .venv
source .venv/bin/activate
pip install west
west init -m https://github.com/savosaicic/zephyr-stts22h --mr main
west update
pip install -r zephyr/scripts/requirements.txt
```

## Device tree overlay

A board overlay is required to enable the I2C bus and declare the STTS22H node.
One is already provided for the **nRF9151 DK**:

```
boards/nrf9151dk_nrf9151_ns.overlay
```

For a different board, create a matching overlay file under `boards/` named
after your board target (e.g. `boards/your_board.overlay`) with the
following content, adjusting the I2C bus node label as needed:

```dts
#include <zephyr/dt-bindings/sensor/stts22h.h>

&arduino_i2c {
    status = "okay";

    stts22h: stts22h@3c {
        compatible = "st,stts22h";
        reg = <0x3c>;
        sampling-rate = <STTS22H_1Hz>;
    };
};
```

## Build & flash

```bash
cd zephyr-stts22h
west build -b nrf9151dk/nrf9151/ns .
west flash
```

Replace `nrf9151dk/nrf9151/ns` with your board target if different.

## Expected serial output

Connect to the board's serial port at **115200 baud**. You should see readings printed every second:

```
[00:00:01.000,000] <inf> zephyr_stts22h: temperature: 23.25 °C
[00:00:02.000,000] <inf> zephyr_stts22h: temperature: 23.26 °C
[00:00:03.000,000] <inf> zephyr_stts22h: temperature: 23.25 °C
```
