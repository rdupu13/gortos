# GORTOS SETUP

## CCS-FREE ON LINUX

this document gives instructions for how to setup a linux/wsl development environment for programming and debugging an MSP430 ez-FET.

no more code composer studio!!


## 1. install dependencies

```bash
sudo apt update
sudo apt install -y build-essential libusb-1.0-0-dev libreadline-dev \
                     pkg-config git wget unzip
```


## 2. install mspdebug

install mspdebug, the CCS replacement!
```bash
git clone https://github.com/dlbeer/mspdebug.git /tmp/mspdebug
cd /tmp/mspdebug
make
sudo make install
cd ..
```
this is the program that flashes the MCU and handles debugging


## 3. install msp430-gcc toolchain (compiler + linker)

move to a temporary folder and download compiler and support files directly from the TI website:
```bash
cd /tmp
wget http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/9_3_1_2/export/msp430-gcc-9.3.1.11_linux64.tar.bz2
wget http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSPGCC/9_3_1_2/export/msp430-gcc-support-files-1.212.zip
```
(links can also be entered into a browser, where they'll be found in ~/Downloads)

create a directory in /opt (or anywhere else), then unzip the tarball.
```bash
sudo mkdir -p /opt/msp430-gcc
sudo tar jxf msp430-gcc-9.3.1.11_linux64.tar.bz2 -C /opt/msp430-gcc --strip-components=1
unzip -o msp430-gcc-support-files-1.212.zip
```

download and copy device-specific linker files into the compiler library:
```bash
sudo cp msp430-gcc-support-files/include/*.ld /opt/msp430-gcc/msp430-elf/lib/
```

add compiler to `PATH` variable (put it in `~/.bashrc` so it persists):
```bash
echo 'export PATH=/opt/msp430-gcc/bin:$PATH' >> ~/.bashrc
source ~/.bashrc
```

check version to verify command-line usage is working:
```bash
msp430-elf-gcc --version
```


## 4. setup udev rules (so `sudo` isn't needed to flash)

eZ-FET exposes 2 device types:
- `usb` device
- `tty` device

`mspdebug` programs through `usb`, but both rules are needed:
```bash
sudo tee /etc/udev/rules.d/45-mspdebug.rules <<'EOF'
SUBSYSTEM=="usb", ATTR{idVendor}=="2047", MODE="0666"
SUBSYSTEM=="tty", ATTRS{idVendor}=="2047", MODE="0666"
EOF
```

led udevadm know about it:
```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```

replug the board!



## 5. WSL ONLY: bind device through USBIPD

on **windows powershell**, install usbipd (if you haven't already):
```bash
winget install usbipd
```

list the current usb devices with `usbipd list`.

find the line containing "ez-FET". for example:
```
6-4   2047:0013   MSP Debug Interface (COM8),
    MSP Application UART1 (COM7)   Not shared
```

bind the device to WSL (if it shows "Not shared" next to it) with the `busid` of YOUR device. for example:
```bash
usbipd bind --busid 6-4
```


## 6. DONE! :)


## 6. gortos: set the platform toggle

open `Makefile` and find the section titled `TOGGLE FOR YOUR PLATFORM:`.

if you are using WSL, set `PLATFORM=wsl`.
if you are using linux, set `PLATFORM=linux`.




## 7. DONE! build and flash

```bash
make flash
```

if it can't find the board, confirm it's visible first: `lsusb | grep 2047`
if ez-FET isn't not listed, it's a hardware/cabling issue, not a toolchain one.
try replugging the board and rerunning `make flash`. sometimes it works the 2nd or 3rd time.
