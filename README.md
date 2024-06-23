# Arduino LED Control

An Arduino-based LED control system with a Linux user-space application. This project enables control of an LED connected to an Arduino Uno via serial commands, allowing for turning the LED on/off, toggling, blinking with adjustable intervals, and reading the current state.

## Features

- **Turn LED On/Off**: Control the LED state.
- **Toggle LED**: Toggle the current state of the LED.
- **Blink LED**: Blink the LED a specified number of times.
- **Adjustable Blink Interval**: Set and get the blink interval in milliseconds.
- **Read LED State**: Retrieve the current state of the LED (on or off).

## Hardware Requirements

- Arduino Uno
- LED
- Resistor (220Ω recommended)
- Breadboard and jumper wires

## Software Requirements

- Arduino IDE
- GCC (GNU Compiler Collection) for compiling the Linux user-space application

## Getting Started

### Arduino Setup

1. Connect the LED to pin 13 of the Arduino Uno with a resistor in series.
2. Upload the provided `led_control.ino` sketch to the Arduino using the Arduino IDE.

### Linux User-Space Application

1. Clone this repository:
   ```sh
   git clone https://github.com/YOUR_USERNAME/arduino-led-control.git
   cd arduino-led-control
   
2. Compile the C program:
```gcc -o led_control src/led_control.c

3. Run the application with the desired command:
```sudo ./led_control on
sudo ./led_control off
sudo ./led_control toggle
sudo ./led_control read
sudo ./led_control blink 5
sudo ./led_control set_interval 200
sudo ./led_control get_interval
