# Coreline Sandbox

Coreline is a C-based embedded application for the Raspberry Pi Pico microcontroller that reads environmental and GPS data and displays it in real time on an SPI LCD.

It integrates a DHT22 sensor for temperature and humidity, a u-blox NEO-6M GPS module for positioning data, and an ST7735S display for output.

## Features
* Temperature and humidity measurement using a DHT22 sensor
* GPS data parsing from a u-blox NEO-6M module
* Real-time display output on an ST7735S SPI LCD
* Continuous loop for live updates
* Lightweight and efficient embedded C implementation

## Hardware Requirements
* Raspberry Pi Pico 2 microcontroller (RP2350 or compatible)
* DHT22 temperature and humidity sensor
* u-blox NEO-6M GPS module
* ST7735S 1.8" SPI LCD (Waveshare or compatible)
* Breadboard and jumper wires

## Build Instructions

Requirements:

* CMake (version 3.13 or newer)
* ARM GCC toolchain (arm-none-eabi-gcc)
* Raspberry Pi Pico 2 SDK

Build steps:

```
git clone https://github.com/hirtenfelder/coreline.git

cd coreline

export PICO_SDK_PATH=/path/to/pico-sdk

mkdir build
cd build
cmake ..
make
```

## Flashing
* Hold the BOOTSEL button on the Pico
* Connect it to your computer via USB
* Copy the generated .uf2 file to the mounted device 
 
## Notes
* The DHT22 sensor requires at least 2 seconds between readings
* GPS modules may take time to acquire a signal
* The current implementation uses a blocking loop
* Error handling is minimal and can be improved

## Future Improvements
* Non-blocking sensor and GPS handling
* Improved error recovery
* Power optimization
* Configurable display layout
* Support for additional sensors
 