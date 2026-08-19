# MSP430 Development Environment Setup

Author: [Ryan Dupuis](https://github.com/rdupu13)

Version 1

## CCS-Free on Linux / WSL

This document gives instructions for how to setup a Linux/WSL development environment for programming and debugging an **MSP430 ez-FET**.

No Code Composer Studio required!


## 1. Install dependencies

```bash
sudo apt update
sudo apt upgrade -y
sudo apt install -y build-essential libusb-1.0-0-dev libusb-dev libreadline-dev pkg-config git wget unzip
```


## 2. Install mspdebug

Install `mspdebug`, the replacement for CCS!
```bash
git clone https://github.com/dlbeer/mspdebug.git /tmp/mspdebug
cd /tmp/mspdebug
make
sudo make install
```
This is the tool that programs the MCU and handles debugging.


## 3. Install msp430-gcc toolchain (compiler + linker)

Move to a temporary folder and download the compiler and support files directly from the TI website:
```bash
cd /tmp
wget http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/9_3_1_2/export/msp430-gcc-9.3.1.11_linux64.tar.bz2
wget http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/9_3_1_2/export/msp430-gcc-support-files-1.212.zip
```
(These links can also be entered into a browser. The files will be found in ~/Downloads)

Create a directory in /opt, then unzip the tarball into it.
```bash
sudo mkdir -p /opt/msp430-gcc
sudo tar jxf msp430-gcc-9.3.1.11_linux64.tar.bz2 -C /opt/msp430-gcc --strip-components=1
```

Unzip and copy device-specific support files into the toolchain:
```bash
unzip -o msp430-gcc-support-files-1.212.zip
sudo cp msp430-gcc-support-files/include/*.ld /opt/msp430-gcc/msp430-elf/lib/
sudo cp msp430-gcc-support-files/include/*.h /opt/msp430-gcc/include/
```

Add compiler to `PATH` variable (put it in `~/.bashrc` so it persists):
```bash
echo 'export PATH=/opt/msp430-gcc/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

Check compiler version to verify command-line usage is working:
```bash
msp430-elf-gcc --version
```
If this line doesn't work, then one of the above commands was executed incorrectly.


## 4. Setup udev rules (so `sudo` isn't needed to flash)

The MSP430 eZ-FET exposes 2 device types:
- `usb` device
- `tty` device

`mspdebug` programs it through `usb`, but both rules are needed:
```bash
sudo tee /etc/udev/rules.d/45-mspdebug.rules <<'EOF'
SUBSYSTEM=="usb", ATTR{idVendor}=="2047", MODE="0666"
SUBSYSTEM=="tty", ATTRS{idVendor}=="2047", MODE="0666"
EOF
```

Let udevadm know about it:
```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Replug your board.


## 5. WSL ONLY: Bind device through usbipd

In **Windows Powershell**, outside of WSL, install usbipd (if you haven't already):
```
winget install usbipd
```

Ensure your MSP430 ez-FET is plugged in. List the current usb devices with `usbipd list`. Find the line containing "ez-FET". For example:
```
6-4   2047:0013   MSP Debug Interface (COM8), MSP Application UART1 (COM7)   Not shared
```
Take note of **your** `busid` at the start of the line.

Bind the device to WSL (if it shows "Not shared" next to it) with the `busid` of **your** device. For example:
```
usbipd bind --busid 6-4
```
Where the `busid` is `6-4`.


## DONE! :)