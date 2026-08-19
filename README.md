# GORTOS

an embedded operating system

### platform: TI MSP430FR2153 microcontroller unit
#### author: [ryan dupuis](https://github.com/rdupu13)

MSP430FR2153 needs an operating system, and gort is up to the task!

no one could ever fathom that gort would ever have an os

and then she did

behold, GORTOS!

gort os is fully functional with driver support for the following peripherals:

* ON-CHIP
    - 4 timer modules (TIMER)
    - universal asynchronous receiver-transmitter (UART)
    - inter-integrated circuit (I2C)
    - serial peripheral protocol (SPI)
    - analog-to-digital converter (ADC)
    - 26 reconfigurable general-purpose I/O pins (SWITCH, LED)
    - general-purpose pattern generator (PATTERNS)
    - pulse-width modulation (PWM, coming soon)

gort os supports a variety of off-chip modules, suitable for any embedded project:

* ON-BOARD
    - CP2102N USB-to-UART
    - MCP7940N real-time clock module (RTC)
    - 23LC1024 128KB serial mini memory manager (MMM, coming soon)
    - more coming soon...


### try gortos yourself: [setup guide](docs/gortos_setup.md)

---------------------------------------

## circuit diagram
![Gort OS Circuit Diagram](docs/assets/gortos_circuit.svg)

## software architecture
![Gort OS Software Architecture](docs/assets/gortos_softarch.svg)

---------------------------------------

P.S. gorb is my bb <3