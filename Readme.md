# Etch-A-Sketch
An electronic etch-a-sketch

## Description 
First cut is to just get the rotary enoder working. Then add a second encoder.  Finally the screen.  In this case
it will be a 128x64 OLED as that is what I have.

Got the initial wireing working and I noticed some inconsistencies with the data.  Reading on the forums, I found a fully interupt driven sample and tried it.

Now to add a second rotary encoder and make it display a graph right underneath the first one in the serial console.

Adding an OLED 128x64 screen with an I2C interface.


## Parts 
- 1 Arduino Nano Every
- 2 CTY1052 Rotary Encoder
- 1 OLED 128x64 screen w/I2C
- 1 Breadboard
- Assorted Jumper wires

## Libraries
- SPI
- Wire
- Adafruit_GFX
- Adafruit_SSD1306

## Wiring
Wire the first rotary encoder to the Arduino

| Enc Pin | Usage | Arduino Pin |
| ------- | ----- | ----------- |
| GND | Ground | GND |
| + | 5 Volts | 5v |
| SW | shaft push | D4 |
| DT | A closure | D3 |
| CLK | B closure | D2 |

Wire the second rotary encoder to the Arduino

| Enc Pin | Usage | Arduino Pin |
| ------- | ----- | ----------- |
| GND | Ground | GND |
| + | 5 Volts | 5v |
| SW | shaft push | D7 |
| DT | A closure | D6 |
| CLK | B closure | D5 |

Wire the OLED Display to the Arduino

| I2C Pin | Usage | Arduino Pin |
| ------- | ----- | ----------- |
| Vcc | 3.3 volts | 3.3V
| GND | Ground | GND |
| SCL | Serial Clock | SCL - A5 |
| SDA | Serial Data | SDA - A4 |


## References ##
- Initial Rotary Encoder code came from [Turtorial of Rotary Encoder with Arduino](https://www.instructables.com/id/Tutorial-of-Rotary-Encoder-With-Arduino/)

- Discussion of missing data from rotary encoder [Aduino Forums](https://forum.arduino.cc/index.php?topic=552990.0)

- Adafruit Libraries can be found in the IDE Library Managerd