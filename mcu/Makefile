MCU      = msp430fr2153

USBIPD = usbipd.exe
MSP_VIDPID = 2047:0013

CC = msp430-elf-gcc
TOOLDIR = /opt/msp430-gcc/msp430-elf

SRCDIR = src
INCDIR = include

EXCLUDE = src/devices/lora.c src/kernel/gfs.c

WARNFLAGS = -Wall -Wno-builtin-declaration-mismatch -Wno-unknown-pragmas -Wno-implicit-function-declaration -Wno-unused-variable -Wno-missing-braces -Wno-comment

CFLAGS  = -mmcu=$(MCU) -Os -fdiagnostics-color=always $(WARNFLAGS) -I $(INCDIR) -I $(TOOLDIR)/include
LDFLAGS = -mmcu=$(MCU) -L $(TOOLDIR)/lib -T $(TOOLDIR)/lib/$(MCU).ld

SRCS   = $(filter-out $(EXCLUDE), $(shell find $(SRCDIR) -name '*.c'))
TARGET = firmware

all: $(TARGET).elf

$(TARGET).elf: $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRCS)

flash: $(TARGET).elf
	@echo "Found MSP430 eZ-FET at BUSID $(BUSID)"
	@$(USBIPD) attach --wsl --busid $(BUSID) 2>/dev/null || true
	@sleep 1
	@sudo mspdebug tilib "prog $(TARGET).elf"

run: $(TARGET).elf
	usbipd.exe attach --wsl --busid 5-4 2>/dev/null || true
	sleep 1
	sudo mspdebug tilib

clean:
	rm -f $(TARGET).elf

.PHONY: all flash clean
