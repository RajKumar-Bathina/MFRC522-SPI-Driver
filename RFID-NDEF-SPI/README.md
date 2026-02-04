# RFID NDEF over SPI (ESP32)

This project is an ESP-IDF example that reads NDEF/NFC tags using an MFRC522 RFID reader over SPI.
It is implemented in C++ and demonstrates tag detection and reading via [`NfcAdapter`](main/NfcAdapter.h).

Features
- Detect NFC tags and print NDEF contents to the serial log.
- Supports MIFARE Classic and Ultralight via [`MFRC522`](main/MFRC522_SPI.h).
- Example main loop is in [`app_main`](main/main.cpp).

Requirements
- ESP32 toolchain and ESP-IDF (use the ESP-IDF version you usually use).
- A wired MFRC522 module connected to the ESP32 (SPI).

Wiring (ESP32 pins used in this project)
- MOSI: [`PIN_NUM_MOSI`](main/MFRC522_SPI.h)
- MISO: [`PIN_NUM_MISO`](main/MFRC522_SPI.h)
- SCLK: [`PIN_NUM_CLK`](main/MFRC522_SPI.h)
- CS:   [`PIN_NUM_CS`](main/MFRC522_SPI.h)
- RST:  [`PIN_NUM_RST`](main/MFRC522_SPI.h)

Build and Flash
1. Configure environment and target as usual for ESP-IDF.
2. Build:
   ```sh
   idf.py build
   ```
3. Flash and monitor:
   ```sh
   idf.py -p /dev/ttyUSBx flash monitor
   ```

Run
- The example's entrypoint is [`app_main`](main/main.cpp). It initializes SPI and the MFRC522 (`[`MFRC522::PCD_Init`](main/MFRC522_SPI.cpp)`) and then polls for tags.
- When a tag is present the program reads it and prints information using [`NfcTag`](main/NfcTag.h).

Files of interest
- [main/main.cpp](main/main.cpp) — example application and loop.
- [main/MFRC522_SPI.h](main/MFRC522_SPI.h), [main/MFRC522_SPI.cpp](main/MFRC522_SPI.cpp) — MFRC522 driver implementation.
- [main/NfcAdapter.h](main/NfcAdapter.h) — adapter that makes tag read/write high-level.
- [main/NfcTag.h](main/NfcTag.h) — tag representation and helpers.
- [LICENSE](LICENSE) — project license (MIT).

License
This project is licensed under the MIT License — see [LICENSE](LICENSE).

# _Sample project_

(See the README.md file in the upper level 'examples' directory for more information about examples.)

This is the simplest buildable example. The example is used by command `idf.py create-project`
that copies the project to user specified path and set it's name. For more information follow the [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project)



## How to use example
We encourage the users to use the example as a template for the new projects.
A recommended way is to follow the instructions on a [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project).

## Example folder contents

This example is implemented in **C++** — all source files are `.cpp`/`.h`. The project contains the example application in `main/main.cpp`, located in the `main/` folder.

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.cpp
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
