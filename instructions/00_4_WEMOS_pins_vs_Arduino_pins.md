# WEMOS pins vs Arduino pins

The full list of output pins to identifiers are as follows, taken from the [source code](https://github.com/esp8266/Arduino/blob/master/variants/d1_mini/pins_arduino.h#L49-L61):

| **WeMos D1 mini Pin Number** | **Arduino IDE Pin Number** |
| ---------------------------- | -------------------------- |
| D0                           | 16                         |
| D1                           | 5                          |
| D2                           | 4                          |
| D3                           | 0                          |
| D4                           | 2                          |
| D5                           | 14                         |
| D6                           | 12                         |
| D7                           | 13                         |
| D8                           | 15                         |
| TX                           | 1                          |
| RX                           | 3                          |

These can be used to program the WeMos D1 mini using the Arduino IDE.