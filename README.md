# Pocket Tester

## Overview

Pocket Tester is an Arduino-based portable electronic component and protocol testing device designed to simplify the testing of commonly used sensors and communication interfaces during embedded systems development.

The device provides a menu-driven interface on an OLED display, allowing users to quickly test multiple sensors and perform basic protocol diagnostics without writing separate test programs for each component.

This project was developed to reduce development time and simplify hardware debugging during electronics and IoT projects.

---

## Features

* OLED-based menu-driven user interface
* Push-button navigation system
* Component testing mode
* Protocol testing mode
* Logic probe functionality
* Simple and portable design
* Easy to extend with additional sensors

---

## Supported Components

* DHT11 Temperature and Humidity Sensor
* HC-SR04 Ultrasonic Sensor
* IR Sensor

---

## Supported Protocol Tests

* GPIO Test
* GPIO Activity Detection
* I2C Device Scanner

---

## Hardware Used

* Arduino Uno
* SSD1306 OLED Display (SPI)
* DHT11 Sensor
* HC-SR04 Ultrasonic Sensor
* IR Sensor
* Push Buttons
* Jumper Wires

---

## Software Used

* Arduino IDE
* C++

---

## Project Structure

```
Pocket-Tester/
│
├── pocket_tester.ino
├── README.md
├── LICENSE
└── .gitignore
```

---

## Applications

* Embedded Systems Development
* Sensor Testing
* Electronics Debugging
* Arduino Learning
* IoT Prototyping

---

## Future Improvements

* Rotary encoder navigation
* TFT display support
* EEPROM-based settings
* SD card logging
* Battery-powered operation
* Additional sensor support
* SPI and UART protocol diagnostics

---

## Author

**Ashwin Bhat**

Electronics and Communication Engineering Student

---

## License

This project is licensed under the MIT License.
