# GORTOS SETUP

this document gives instructions for setting up gortos on a Linux or WSL platform. **before following these steps**, click [here](msp430_dev_setup.md) for instructions on how to set up the MSP430 development environment.


## 1. modify makefile settings

open `Makefile` and find the section titled `MODIFY FOR YOUR PLATFORM:`.

if you are using WSL, set `PLATFORM=wsl`.
if you are using linux, set `PLATFORM=linux`.


## 2. build and flash

assuming you **already** installed mspdebug and msp430-elf-gcc, you can now run:
```bash
make flash
```
to load gortos to your board.

if it can't find the board, confirm it's visible first: `lsusb | grep 2047`.

if ez-FET isn't not listed, it's a hardware/cabling issue, not a toolchain one.

try replugging the board and rerunning `make flash`. sometimes it works the 2nd or 3rd time.
