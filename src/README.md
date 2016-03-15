# Sources

- **Arduino**: The Arduino based sources for the 23CM transceiver. Program these into
  an ATMEGA328 chip and place it into your 23CM Transceiver to get the new Arduino
  version of the firmware. Feel free to contribute to this repository.
- **AVR-Studio**: The original sources as programmed into the ATMEGA328 that came with
  version 2.2 of the 23CM NBFM transceiver. These sources are here for reference only.
  The ownership and source control is managed by the original owner, please contact 
  [Bas PE1JPD](http://www.pe1jpd.nl/index.php/23cm_nbfm/) for the latest version or questions.

## Pin mappings

Below are the mappings for connecting your Arduino to the atmel socket on the respective
23cm trx boards. If the software does not work, please double check these connections:

| Arduino  |  Board v1 | Board v2.1 |             |
| Pro Mini |      IC11 |       IC11 | Function    |
|---------:|----------:|-----------:|-------------|
|      TX0 |        nc |         nc |             |
|      RX1 |        nc |         nc |             |
|      RST |        nc |         nc |             |
|      GND |         8 |          8 |             |
|        2 |         4 |          5 | Rotary A    |
|        3 |         5 |          6 | Rotary B    |
|        4 |         6 |         18 | LCD D4      |
|        5 |        11 |         19 | LCD D5      |
|        6 |        12 |          9 | LCD D6      |
|        7 |        13 |         10 | LCD D7      |
|        8 |        14 |          2 | PTT         |
|        9 |        15 |          4 | Rotary Push |
|       10 | Backlight |  Backlight | Dimmer      |
|       11 |        17 |         15 | LCD RS      |
|       12 |        16 |         14 | LCD LE      |
|       13 |        19 |         17 | CTCSS out   |
|       A0 |        27 |         27 | Mute audio  |
|       A1 |        28 |         28 | MC3362 RSSI |
|       A2 |        26 |         26 | TX on       |
|       A3 |        25 |         23 | PLL Clock   |
|      VCC |         7 |         21 |             |
|       A4 |        24 |         25 | PLL Data    |
|       A5 |        23 |         24 | PLL LE      |
|       A6 |        nc |         nc |             |
|       A7 |        nc |         nc |             |
|      RAW |        nc |         nc |             |

Connecting pin 10 to the Backlight LED of your LCD panel is optional.
