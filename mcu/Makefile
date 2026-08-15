MCU = msp430fr2153

USBIPD = usbipd.exe
MSP_VIDPID = 2047:0013

CC = msp430-elf-gcc
TOOLDIR = /opt/msp430-gcc/msp430-elf

SRCDIR = src
INCDIR = include

EXCLUDE = 	src/kernel/gfs.c \
			include/kernel/gfs.h \
			src/devices/dial.c \
			include/devices/dial.h \
			src/devices/lcd.c \
			include/devices/lcd.h \
			src/devices/lora.c \
			include/devices/lora.h \
			src/devices/mmm.c \
			include/devices/mmm.h \
			src/devices/pwm.c \
			include/devices/pwm.h

WARNFLAGS = -Wall \
			-Wno-builtin-declaration-mismatch \
			-Wno-unknown-pragmas \
			-Wno-implicit-function-declaration \
			-Wno-missing-braces \
			-Wno-comment

CCFLAGS =	-mmcu=$(MCU) \
			-Os \
			-fdiagnostics-color=always \
			$(WARNFLAGS) \
			-I $(INCDIR) \
			-I $(TOOLDIR)/include

LDFLAGS = 	-mmcu=$(MCU) \
			-L $(TOOLDIR)/lib \
			-T $(TOOLDIR)/lib/$(MCU).ld

SRCS =		$(filter-out $(EXCLUDE), \
			$(shell find $(SRCDIR) -name '*.c'))

TARGET = gort

all: $(TARGET).out

$(TARGET).out: $(SRCS)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $(SRCS)

flash: $(TARGET).out
	@echo "Found MSP430 eZ-FET at BUSID $(BUSID)"
	@$(USBIPD) attach --wsl --busid $(BUSID) 2>/dev/null || true
	@sleep 1
	@sudo mspdebug tilib "prog $(TARGET).out"

debug: $(TARGET).out
	@echo "Found MSP430 eZ-FET at BUSID $(BUSID)"
	@$(USBIPD) attach --wsl --busid $(BUSID) 2>/dev/null || true
	@sleep 1
	@sudo mspdebug tilib

clean:
	rm -f $(TARGET).out

.PHONY: all flash debug clean
