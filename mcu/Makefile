# =============================================================================
# GORTOS MAKEFILE
# 
# created by rdupu13
# =============================================================================

# COMPILER & TOOLCHAIN ------------------------------------
CC = msp430-elf-gcc
TOOLDIR = /opt/msp430-gcc/msp430-elf
# ---------------------------------------------------------

# BUILD CONFIGURATION ---------------------------------------------------------

# TOGGLE FOR YOUR PLATFORM: -----------
PLATFORM = wsl
MCU = msp430fr2153
# -------------------------------------

INCDIR = include
SRCDIR = src
TEST_SRCDIR = test

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
# -----------------------------------------------------------------------------

# WSL-ONLY ----------------------------
ifeq ($(PLATFORM), wsl)
USBIPD = usbipd.exe
MSP_VIDPID = 2047:0013
BUSID := $(shell \
			$(USBIPD) list 2>/dev/null \
			| grep '$(MSP_VIDPID)' \
			| awk '{print $$1}')
ATTACH = attach-fet
else
ATTACH = 
endif
# -------------------------------------

# COMPILE GORT! ===========================================
all: $(TARGET).out
$(TARGET).out: $(SRCS)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $@ $(SRCS)
# =========================================================

# USBIPD ATTACH FET ---------------------------------------
attach-fet:
	@if [ -z "$(BUSID)" ]; then \
		echo "usbipd: no MSP430 eZ-FET found."; \
		exit 1; \
	fi
	@echo "usbipd: found MSP430 eZ-FET at bus ID: $(BUSID)"
	$(USBIPD) attach --wsl --busid $(BUSID) 2>/dev/null || true
	@sleep 1
# ---------------------------------------------------------

# TEST ------------------------------------------------------------------------
test:
	@$(MAKE) SRCDIR=$(TEST_SRCDIR) TARGET=test
# -----------------------------------------------------------------------------

# FLASH -----------------------------------------------------------------------
flash: $(TARGET).out $(ATTACH)
	mspdebug tilib "prog $(TARGET).out"
# -----------------------------------------------------------------------------

# TEST FLASH ------------------------------------------------------------------
test-flash:
	@$(MAKE) flash SRCDIR=$(TEST_SRCDIR) TARGET=test
# -----------------------------------------------------------------------------

# DEBUG -----------------------------------------------------------------------
debug: $(TARGET).out $(ATTACH)
	mspdebug tilib
# -----------------------------------------------------------------------------

# CLEAN -----------------------------------------------------------------------
clean:
	rm -f *.out
# -----------------------------------------------------------------------------

.PHONY: all test flash test-flash debug clean attach-fet