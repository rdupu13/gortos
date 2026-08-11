# --- Edit this for your part, e.g. msp430g2553, msp430fr5969 ---
MCU      = msp430fr2153

CC       = msp430-elf-gcc
TOOLDIR  = /opt/msp430-gcc/msp430-elf

SRCDIR   = src
INCDIR   = include

# --- Files to leave out of the build (paths relative to project root) ---
EXCLUDE  = src/devices/lora.c src/kernel/gfs.c

# --- Warning flags: add -Wno-<name> to suppress specific warnings ---
WARNFLAGS = -Wall -Wno-builtin-declaration-mismatch -Wno-unknown-pragmas -Wno-implicit-function-declaration -Wno-unused-variable -Wno-missing-braces -Wno-comment

CFLAGS   = -mmcu=$(MCU) -Os $(WARNFLAGS) -I $(INCDIR) -I $(TOOLDIR)/include
LDFLAGS  = -mmcu=$(MCU) -L $(TOOLDIR)/lib -T $(TOOLDIR)/lib/$(MCU).ld

# Recurse into all subdirectories under SRCDIR, then drop excluded files
SRCS     = $(filter-out $(EXCLUDE), $(shell find $(SRCDIR) -name '*.c'))
TARGET   = firmware

all: $(TARGET).elf

$(TARGET).elf: $(SRCS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRCS)

flash: $(TARGET).elf
	sudo mspdebug tilib "prog $(TARGET).elf"

run: $(TARGET).elf
	sudo mspdebug tilib

clean:
	rm -f $(TARGET).elf

.PHONY: all flash clean
