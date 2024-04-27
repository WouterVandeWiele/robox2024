# Robox 2024

## Configuration

main.cpp is configurable through defines to select different examples or components.


### Audio components

Configuration of the arduino audio library components. 
In the context of the robox project they all drive the I2S amp and thus only one can be enabled at a time.

Select one of:

```h
#define ROBOX_FULL          //  the complete audio project, with audio mux

// the examples, from the arduino audio library, with minimal adaptations
#define ROBOX_EXAMPLE_BLE
#define ROBOX_EXAMPLE_SD
#define ROBOX_EXAMPLE_WEB

// robox components, based on the examples above, with adaptations for the audio mux
#define ROBOX_COMPONENT_BLE
#define ROBOX_COMPONENT_WEB
#define ROBOX_COMPONENT_SD
```

### addon components

Additional functionality can be included or removed, multiple can be selected:
```h
#define ROBOX_LCD         // include the LCD component (headers, init and loop)
#define ROBOX_DEBUG_CLI   // include the debug CLI component (headers, init and loop)
```

### wifi credentials

In order to use the webstream component, wifi credentials need to be added to access the web. Follow the instructions here: [wifi credentials](lib/WIFI_CREDENTIALS/README.md)


## Debug CLI

To ease manual testing a debug CLI is added (configurable through the preprocessor statements)
Open a serial connection and when the the ESP starts, follozing messages are displayed if the CLI is initialized.

```
>>> Starting debug CLI.
>Ping was added to the CLI!
>Help was added to the CLI!
>Print was added to the CLI!
>Source was added to the CLI!
>>> CLI installed, type "help" for all options.
```

Currently following options are implemented:
```
help
# help
ping
Replies with "pong" if the ESP32 is still going through the main loop

help
Print cli help, lists all registered commands

print -text <value> [-n <1>]
Example command demonstrating aruments, takes <text> and prints n times

source -s <value>
Select the audio source, takes 1 arument of: ['ble', 'web', 'sd']
```

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