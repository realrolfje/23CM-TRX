# Turning the 23cm TRX into an Arduino

This directory contains the sources and instructions to replace IC11 with an Arduino
Pro Mini 5V or compatible.

## Loading the sources

To load this project into your Arduino you need the
[Arduino 1.6.8 IDE](https://www.arduino.cc/en/Main/Software) or later. After installing
the IDE, please copy the files in the [libraries](libraries) folder into the library
folder of your IDE.

Open the project in the Arduino IDE by opening the _23cm-trx.ino file. Please do not
rename the folder or the file. The underscore is a workaround for the fact that this
version of the Arduino IDE can not handle filenames starting with a number.

When the project is loaded, you will find a number of tabs at the top of the screen. Each
tab contains the code for specific functions.

## Programming the Arduino

In the Arduino IDE:

1. select "Tools" -> "Board" -> "Arduino Pro or Pro Mini". 
2. select the correct voltage with "Tools" -> "Processor" -> "ATmega328 (5V, 16MHz)".
3. select the correct Serial connection in "Tools" ->  "Port".

Select the upload botton (second green button from the top left) and check the text in
the output window.

## Pin mappings

Below are the mappings for connecting your Arduino Pro Mini or compatible to the socket of IC11 on the respective 23cm trx board versions. The version of the 23cm board is on the
bottom side, near the antenna connector and IC1.

**If the software does not work, please double check these connections:**

| Arduino Pro Mini |  IC11 Board v1 | IC11 Board v2.1 | Function    |
|-----------------:|---------------:|----------------:|-------------|
|              TX0 |             nc |              nc |             |
|              RX1 |             nc |              nc |             |
|              RST |             nc |              nc |             |
|              GND |              8 |               8 |             |
|                2 |              4 |               5 | Rotary A    |
|                3 |              5 |               6 | Rotary B    |
|                4 |              6 |              18 | LCD D4      |
|                5 |             11 |              19 | LCD D5      |
|                6 |             12 |               9 | LCD D6      |
|                7 |             13 |              10 | LCD D7      |
|                8 |             14 |               2 | PTT         |
|                9 |             15 |               4 | Rotary Push |
|               10 |      Backlight |       Backlight | Dimmer      |
|               11 |             17 |              15 | LCD RS      |
|               12 |             16 |              14 | LCD LE      |
|               13 |             19 |              17 | CTCSS out   |
|               A0 |             27 |              27 | Mute audio  |
|               A1 |             28 |              28 | MC3362 RSSI |
|               A2 |             26 |              26 | TX on       |
|               A3 |             25 |              23 | PLL Clock   |
|              VCC |              7 |              21 |             |
|               A4 |             24 |              25 | PLL Data    |
|               A5 |             23 |              24 | PLL LE      |
|               A6 |             nc |              nc |             |
|               A7 |             nc |              nc |             |
|              RAW |             nc |              nc |             |

Connecting pin 10 to the Backlight LED of your LCD panel is optional.
