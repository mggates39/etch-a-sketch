# Etch-A-Scketc
An electronic etch-a-sketch

## Description 
First cut is to just get the rotary enoder working. Then add a second encoder.  Finally the screen.  In this case
it will be a 128x64 OLED as that is what I have.


## Parts 
- 1 Arduino Nano Every
- 1 CTY1052 Rotary Encoder
- 1 Breadboard
- Assorted Jumper wires

## Wireing
Initially wire the rotary encoder to the Arduino

| Enc Pin | Usage | Arduino Pin |
| ------- | ----- | ----------- |
| GND | Ground | GND |
| + | 5 Volts | 5v |
| SW | shaft push | D4 |
| DT | A closure | D3 |
| CLK | B closure | D2 |

## References ##
Initial Rotary Encoder code came from [Turtorial of Rotary Encoder with Arduino](https://www.instructables.com/id/Tutorial-of-Rotary-Encoder-With-Arduino/)
