# Gort OS - an Embedded Operating System
#### Author: [Ryan Dupuis](https://github.com/rdupu13)

## Platforms:
* **TI MSP430FR2153 microcontroller unit**

---
```
MSP430FR2153 needs an operating system, and gort is up to the task!

no one could ever fathom that gort would ever have an os

behold, GORTOS!
```
---

## [Features](docs/planning/gortos.md)
Gort OS is fully functional with driver support for the following peripherals:

* 4 timer modules (TIMER)
* universal asynchronous receiver-transmitter (UART)
* inter-integrated circuit (I2C)
* serial peripheral protocol (SPI)
* analog-to-digital converter (ADC)
* 26 reconfigurable general-purpose I/O pins (SWITCH, LED)
* general-purpose pattern generator (PATTERNS)
* pulse-width modulation (PWM, coming soon)

Gort OS supports a variety of breadboard modules, suitable for nearly any embedded project:

* CP2102N USB-to-UART
* MCP7940N real-time clock module (RTC)
* 23LC1024 128KB serial mini memory manager (MMM)
* more soon...


## Development

Follow instructions [[here](https://github.com/rdupu13/msp430-dev/blob/main/msp430_dev_setup.md)] for how to setup your own MSP430 development environment on Linux or WSL. No Code Composer Studio required!

## EELE 465

This project was created in part to fulfill the requirements of the MSU course [EELE 465 - Microcontroller Applications](https://catalog.montana.edu/coursedescriptions/eele/). Below are reports detailing the development and testing of several components of the Gort System:

* Lab 0: [Heartbeat LED](mcu/test/main.c) (circa. 2024)
* Lab 1: GitHub and Markdown (circa. 2025)
* Lab 2: [I2C Bit-Banging](docs/labs/lab2_i2c_bb.md) (8/24/2026)
* Lab 3: [RTC and LED Patterns](docs/labs/lab3_rtc_patterns.md) (~7/12/2026 - 7/26/2026)
* Lab 4: [ADC and SPI](docs/labs/lab4_adc_spi.md) (8/9/2026 - ~8/30/2026)
* Lab 5: [LCD and DAC](docs/labs/lab5_lcd_dac.md) (~9/1/2026 - 11//2026)
* Lab 6: [Dial and Menu](docs/labs/lab6_dial_menu.md) (11//2026)
* Final Project: [Serial RAM and File System Interface](docs/planning/final_project_proposal.md) (p. 11/9/2026)

[Course BOM](docs/planning/eele465_bom.md)

## Breadboard
![Gort OS Breadboard](docs/assets/photo/board10.jpg)

## Circuit Diagram
![Gort OS Circuit Diagram](docs/assets/gortos_circuit.svg)

## Software Architecture
![Gort OS Software Architecture](docs/assets/gortos_softarch.svg)

---
P.S. gorb is my bb <3