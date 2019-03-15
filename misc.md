## references / code used

- https://css-tricks.com/old-timey-terminal-styling/
- https://github.com/jamesalvarez/draggable-piechart
- http://cs107e.github.io/guides/UART-WiFi-Bridge/
- https://github.com/cs107e/cs107e.github.io/blob/master/cs107e/src/rand.c

## pinout

|    Pin    |   Usage   |
| --------  | --------- |
|  0 ID_SD  |           |
|  1 ID_SD  |           |
|  2 SDA    |           |
|  3 SCL    | Magstripe |   
|  4 GPCLK0 | Magstripe |
|  5        |           |
|  6        | Valves    |
|  7 CE1    |           |
|  8 CE0    | ADC       |
|  9 MISO   | ADC       |
| 10 MOSI   | ADC       |
| 11 SCLK   | ADC       |
| 12 PWM0   | Stepper   |
| 13 PWM1   | Valves    |
| 14 TXD    | UART      |
| 15 RXD    | UART      |
| 16        | Stepper   |   
| 17        | Button    |
| 18 PWM0   |           |
| 19 MISO   | Valves    |
| 20 MOSI   | Stepper   |
| 21 SCLK   | Stepper   |
| 22        |           |
| 23        | TAJ       |
| 24        | TAJ       |
| 25        |           |
| 26        | Valves    |
| 27        |           |

|      Left      |     Right     |
| -------------- | ------------- |
| (3.3V) Power   | (5V) Power    |
| (2)            | (5V) Power    |
| (3) Magstripe  | (GND)         |
| (4) Magstripe  | (14) UART     |
| (GND)          | (15) UART     |
| (17) Button    | (18)          |
| (27) Magstripe | (GND)         |
| (22)           | (23) TAJ      |
| (3.3V) Power   | (24) TAJ      |
| (10) ADC       | (GND)         |
| (9) ADC        | (25)          |
| (11) ADC       | (8) ADC       |
| (GND)          | (7)           |
| (0)            | (1)           |
| (5)            | (GND)         |
| (6) Valves     | (12) Stepper  |
| (13) Valves    | (GND)         |
| (19) Valves    | (16) Stepper  |
| (26) Valves    | (20) Stepper  |
| (GND)          | (21) Stepper  |
