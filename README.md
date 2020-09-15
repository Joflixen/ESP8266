# ESP-01 (ESP8266EX) Projects
This repository contains a bunch of ill concieved ESP-01 MCU projects.

## Board Layout for ESP-01 Form Factor
The logical form factor of the ESP-01 board is 14.3mm x 24.8mm with an 8-pin header at one end and a PCB antenna at the other.

*Note: According to the technical specifications this entenna is 3dBi*

| GND 	| GPIO2	| GPIO0 	| RXD 	|
|-----	|------	|-------	|-----	|
|  1  	|   2  	|   3   	|  4  	|
|  8  	|   7  	|   6   	|  5  	|
| TXD 	| CHIP  | RESET 	| VCC 	|

## ESP8266 Connection to ESP-01 Pinout
The actual ESP8266EX MCU on this board only has some of it's pinouts broken out on the board, these are as follows:

| Connection          | ESP-01        | ESP-01S     |
| ------------------- | ------------- | ----------- |
| Power Ground        | Pin 1         | Pin 1       |
| GPIO 2 +            | Pin 2   | Pin 2 + Blue LED  |
| GPIO 0 + SPI        | Pin 3         | Pin 3       |
| GPIO 3 + UART0 RXD  | Pin 4         | Pin 4       |
| 3.3v Power In       | Pin 5         | Pin 5       |
| External Reset      | Pin 6         | Pin 6       |
| Chip Enable (High)  | Pin 7         | Pin 7       |
| GPIO 1 + UART0 TXD  | Pin 8 + Blue LED  | Pin 8   |


