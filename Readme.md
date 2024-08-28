# Robox 2024 (update is nearly there)

![image](https://github.com/user-attachments/assets/fac3a691-4a6e-4169-82d3-f7d4ebb7cdb1)

English version below.

Welkom bij het Robox 2024 (v2) project. Hier vind je alle informatie omtrent dit project. Voor de aanloop van FRI3D hebben we al wat basisinformatie gepubliceerd om je wegwijs te maken met de radio. In de weken na het kamp gaan we nog wat updates toevoegen om de functionaliteit van Robox v2 te verbeteren en uit te breiden. Dus hou deze website in het oog. Er zal dan uitgelegd worden hoe je zelf de udpate kan uitvoeren. Om deze website terug te vinden volstaat het om de QR-code op de achterkant van de radio te scannen.

- status update: de eerste update is bijna klaar en zal binnenkort vrijgegeven worden. Binnenkort geven we hierover meer details. We gaan ook de documentatie op deze site uitbreiden en trachten in het nederlands en engels aan te bieden.
- **Er zijn nog 4 kits beschikbaar** en deze kunnen geassembleerd worden tijdens een **coderdojo workshop in westerlo (14/09)** best om rechtstreeks contact op te nemen met de coderdojo van westerlo (per mail) om een plaatsje te reserveren.
- Voor degenen die het project nog verder willen steunen of een bijdrage willen leveren mogen een donatie doen via https://buymeacoffee.com/erowin
- Heb je vragen over het project kan je via github een issue openen of ons contacteren via discord in deze server: https://discord.gg/HG2vBjpU9c (vragen naar @erowin of @wally)

Welcome at the Robox 2024 (v2) project, here you will find all information about this project. Before FRI3D we have already published some basic information to get to know your radio. In the next comming weeks after the camp we will add some firmware updates to increase the functionality of Robox V2. So keep an eye on this site. There will appear an explanation on how to execute the update all by yourself. To find back this webpage you can scan the QR-code on the back of the radio.

- update: the first firmware update is almost ready and will be released soon. We will add the release notes soon as well. We will also extend the documentation a bit and we will try to provide a dutch and english translation of the documentation.

- For the ones who want to support the project can donate here: https://buymeacoffee.com/erowin
- If you have questions about the project you can contact us on discord in this server: https://discord.gg/HG2vBjpU9c (you can ask for @erowin or @wally)

## Handleiding

### Beschrijving

![image](https://github.com/user-attachments/assets/3a1b787e-b27b-40e4-957e-7b2eefd091d8)

1)	Aan/Uit-schakelaar: kan je de luidspreker mee aan of uitzetten. Om te laden dient deze aan te staan.
2)	Menubediening (knop): dient om door de menu’s op het scherm te navigeren. (“bron selectie”, “afspelen”, “instellingen”)
3)	Muziekbediening (knop): dient om de muziek te kunnen bedienen.

![image](https://github.com/user-attachments/assets/0f5baad6-b415-4467-99e2-28f18f9649bb)

Functies:
 
De luidspreker kan muziek afspelen via:

•	Bluetooth

•	Wifi (webradio) 

•	microSD-kaart (.mp3)

•	geen muziekbron

1.	Party modes:

•	Leds op het ritme van de muziek.

•	Wielen die bewegen op het ritme van de muziek.

•	Combinatie van beide.

2.	Batterij:

•	Bijna leeg ==> batterij icoon gaat aan

•	Leeg ==> batterij icoon knippert

•	Opladen wordt aangeduid door een knipperend “zonnetjes” symbool. Als de batterij volgeladen is, dan brandt het “zonnetjes” symbool gewoon.

3.	Menu-items (te bedienen met de menu-knop)
   
![image](https://github.com/user-attachments/assets/a7f4598d-2d58-48c1-abd4-261a29206109)

### Audiobronnen

Van audiobron wisselen doe je door naar het menu “Bron selectie” te gaan (symbool “dubbele gekruiste pijl). Volgende bronnen zijn beschikbaar:

•	Internetradio ==> Hierbij kan je kiezen tussen verschillende internetradiostations. 

  o	Als je “internetradio” kiest dan zal deze eerst proberen te verbinden met al gekende netwerken. Als deze niets vindt, of het lukt niet, dan zet deze zijn eigen wifi-netwerk op genaamd: “ROBOX-XXXXXXX”. (Zolang de “wifi” niet ingesteld is blijft deze op het scherm vasthangen). Je kan met je gsm/laptop verbinden met het wifi-netwerk van ROBOX, hier kan je dan het gewenste wifi-netwerk toevoegen waarmee ROBOX verbinding moet maken. Normaalgezien opent de webbrowser vervolgens automatisch een website (zie afbeelding rechts). Indien dit niet het geval is, dan kan je het volgende manueel in de adresbalk ingeven: http://192.168.4.1 Op deze website kan je dan het wifi-netwerk kiezen en vervolgens opslaan “save”. ROBOX zal proberen te verbinden met dit netwerk. Als dit niet lukt, dan zet deze terug zijn eigen wifi aan zodat je het opnieuw kan ingeven.

![image](https://github.com/user-attachments/assets/e9874abe-65e9-402e-ac94-a2c42a5bf81b)


  o	Je kan zelf radiostations toevoegen door naar “Geen bron” te gaan en dan bij “Instellingen” kan je “Webconfiguratie” kiezen. Dan moet je terug surfen met je laptop naar ip adres dat bij in de instellingen staat. Bijvoorbeeld: http://192.168.1.183 in de adresbalk van je webbrowser ingeven.

  ![image](https://github.com/user-attachments/assets/317f0081-e539-452f-8497-3c02b377fa91)

•	Bluetooth ==> Hierbij kan je bluetooth verbinding maken met een gsm/laptop en muziek afspelen. De bluetooth naam vindt je in de “Instellingen”.

•	microSD-kaart (mp3-speler) ==> Hierbij kan je muziek afspelen van een microSD-kaart.

•	Geen bron ==> Dan speelt ROBOX geen muziek af, maar kan je extra instellingen doen via “Instellingen” en “Webconfiguratie” door vervolgens te surfen naar het ip-adres dat ook in “Instellingen” staat. (zie afbeelding rechts)


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

mux -meta_title -volume <value>
(get/set mux settings)
-meta_title: reads the current title of the song
-volume: sets the colume on the stream
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
https://atctwo.net/projects/esp32-partition-editor/

## burn pip install reedsolo

```shell
pip install cryptography
pip install ecdsa
pip install bitstring
pip install reedsolo
```

PlatformIO Core CLI:
```shell
pio pkg exec -- espefuse.py set_flash_voltage 3.3V --port COM3
```
