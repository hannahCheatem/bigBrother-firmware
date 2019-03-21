# Big Brother Firmware

The microcontroller code base for Big Brother

Requires:
    * Software:
        * PlatformIO  - IDE to manage build/uploading
        * Version - 3.6.5 (core) and 2.0.0 (home)
          * C++ Settings:
                * C++11   - Standard Used
                * avr-gcc - Compiler/upload tools
        * Adafruit_FONA - Library to use FONA 808 (lib managed by PlatformIO)
    * Hardware:
        * Arduino Uno - Microcontroller Platform
        * Adafruit FONA 808 - GPS/GSM Module
        * Li-Ion Battery
        * GSM Antenna (Passive)
        * GPS Antenna (Passive)
    