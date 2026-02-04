# RFID NDEF over SPI (ESP32)
This project is an ESP-IDF example that reads NDEF/NFC tags using an MFRC522 RFID reader over SPI.  
It is implemented in C++ and demonstrates tag detection, NDEF parsing, and data exchange via [`NfcAdapter`](main/NfcAdapter.h).

## Features
- Detect NFC tags and print NDEF contents to the serial log.
- Supports NDEF data exchange including URL, plain text, Wi-Fi credentials, and other standard record types.
- Supports MIFARE Classic and Ultralight via [`MFRC522`](main/MFRC522_SPI.h).
- Example main loop is in [`app_main`](main/main.cpp).

## Requirements
- ESP32 toolchain and ESP-IDF (use the ESP-IDF version you usually use).
- A wired MFRC522 module connected to the ESP32 (SPI).

## Wiring (ESP32 pins used in this project)
- MOSI: [`PIN_NUM_MOSI`](main/MFRC522_SPI.h)
- MISO: [`PIN_NUM_MISO`](main/MFRC522_SPI.h)
- SCLK: [`PIN_NUM_CLK`](main/MFRC522_SPI.h)
- CS:   [`PIN_NUM_CS`](main/MFRC522_SPI.h)
- RST:  [`PIN_NUM_RST`](main/MFRC522_SPI.h)

## Build and Flash
1. Configure environment and target as usual for ESP-IDF.
2. Build:
   ```sh
   idf.py build


Run
- The example's entrypoint is [`app_main`](main/main.cpp). It initializes SPI and the MFRC522 (`[`MFRC522::PCD_Init`](main/MFRC522_SPI.cpp)`) and then polls for tags.
- When a tag is present the program reads it and prints information using [`NfcTag`](main/NfcTag.h).

License
This project is licensed under the MIT License
