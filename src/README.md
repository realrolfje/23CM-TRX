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

| Arduino | Board 2.1 | Function    |
|--------:|----------:|-------------|
|     TX0 |        nc |             |
|     RX1 |        nc |             |
|     RST |        nc |             |
|     GND |         8 |             |
|       2 |         5 | Rotary A    |
|       3 |         6 | Rotary B    |
|       4 |        18 | LCD D4      |
|       5 |        19 | LCD D5      |
|       6 |         9 | LCD D6      |
|       7 |        10 | LCD D7      |
|       8 |         2 | PTT         |
|       9 |         4 | Rotary Push |
|      10 | Backlight |             |
|      11 |        15 | LCD RS      |
|      12 |        14 | LCD LE      |
|      13 |        17 | CTCSS out   |
|      A0 |        27 | Mute audio  |
|      A1 |        28 | MC3362 RSSI |
|      A2 |        26 | TX on       |
|      A3 |        23 | PLL Clock   |
|     VCC |        21 |             |
|      A4 |        25 | PLL Data    |
|      A5 |        24 | PLL LE      |
|      A6 |        nc |             |
|      A7 |        nc |             |
|     RAW |        nc |             |

Connecting pin 10 to the Backlight LED of your LCD panel is optional.
