# Robox 2024

## Working examples

main.cpp

- #include "player_metadata.h"
- #include "sdfat_example.h" // https://www.pschatzmann.ch/home/2021/11/02/streaming-mp3-files-to-a-bluetooth-speaker/
  -> not using the SD functionality, comment "https://github.com/greiman/SdFat.git@^2.2.2" in the lib_deps of platformio.ini

- #include "ble_example.h"


## PlatformIO configuration (dev log)
### Freenove fnk0085
https://community.platformio.org/t/esp32s-configuration-error/38647/2

Copy the freenove_fnk0085.json to the projects boards folder.

`.platformio\platforms\espressif32\boards`


## JTAG debugger (ESP32-WROOM-DA)

https://www.youtube.com/watch?v=ENLhW0MQuu0

----

https://www.reddit.com/r/esp32/comments/pvidlt/rst0x10_rtcwdt_rtc_resetboot0x33_spi_fast_flash/

It's always good to read the datasheet. GPIO12 (aka MTDI) is a strapping pin, and when pulled low at boot sets the internal voltage regulator (VDD_SDIO) to 1.8 V. Most dev boards use 3.3 V flash, so... 

----

https://www.reddit.com/r/esp32/comments/yw1w0a/cant_tell_what_name_will_this_show_with_in/

It will work because it's based on the ESP32 generic. The problem is that the DA module has the RF switch connected to 2 different GPIOs that the Wi-Fi API controls.

Be sure you are not using GPIO2 and GPIO25 when using the DA module.

## ESP32 partition calculator
https://esp32.jgarrettcorbin.com/