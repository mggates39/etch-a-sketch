# Etch-A-Scketc
An electronic etch-a-sketch

## Description 
First cut is to just get the rotary enoder working. Then add a second encoder.  Finally the screen.  In this case
it will be a 128x64 OLED as that is what I have.

Got the initial wiring working and I noticed some inconsistencies with the data.  Reading on the forums, I found a fully interupt driven sample and tried it.

Further digging found a library that directly supports the encoder processing.  I used that to bring in the second encoder with minimal changes.


## Parts 
- 1 Arduino Nano Every
- 2 CTY1052 Rotary Encoder
- 1 Breadboard
- Assorted Jumper wires

## Wiring
Initially wire the first rotary encoder to the Arduino

| Enc Pin | Usage | Arduino Pin |
| ------- | ----- | ----------- |
| GND | Ground | GND |
| + | 5 Volts | 5v |
| SW | shaft push | D4 |
| DT | B closure | D3 |
| CLK | A closure | D2 |

Then wire the second rotary encoder to the Arduino

| Enc Pin | Usage | Arduino Pin |
| ------- | ----- | ----------- |
| GND | Ground | GND |
| + | 5 Volts | 5v |
| SW | shaft push | D7 |
| DT | B closure | D6 |
| CLK | A closure | D5 |

## References ##
- Initial Rotary Encoder code came from [Turtorial of Rotary Encoder with Arduino](https://www.instructables.com/id/Tutorial-of-Rotary-Encoder-With-Arduino/)

- Discussion of missing data from rotary encoder [Aduino Forums](https://forum.arduino.cc/index.php?topic=552990.0)

- The Encoder Library is available from the library managar.  Examples and discussion can be found [here](https://www.pjrc.com/teensy/td_libs_Encoder.html)