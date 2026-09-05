# Lab 4: ADC and SPI

#### Ryan Dupuis
#### EELE 465 | 9//2026

## Introduction

In this lab, 

All source files are located here: [mcu/src/](../../mcu/src/).

## Circuit Diagram
### Requirements met: 
![Lab 4 Circuit Diagram](../assets/lab4/lab4_circuit.svg)


## Analog-to-Digital Conversion
### Requirements met: 

The driver for Gort's [ADC peripheral](../../mcu/src/drivers/adc.c)

```c

```

![]()


## Temperature Sensor Driver
### Requirements met: 

The driver for Gort's [temperature sensors](../../mcu/src/devices/temp.c)

```c

```

![]()


## SPI Driver
### Prerequisite for LED Stick

The [SPI Driver](../../mcu/src/drivers/spi.c)

```c
unsigned char b = 0x4D;
unsigned char s;
unsigned int i;
for (i = 0; i < 8; i++)
{
    s = (b & 0x80);
    LEDSTICK_PORT &= ~LEDSTICK_PIN;
    if (s) {
        __asm__ __volatile__("nop");
        LEDSTICK_PORT |= LEDSTICK_PIN;
        b = b << 1;
    }
    else {
        LEDSTICK_PORT |= LEDSTICK_PIN;
        b = b << 1;
    }
}
```

Maybe try assembly next?



![]()


## LED Stick Driver
### Requirements met:

The [LED Stick](../../mcu/src/devices/stick.c)

```c

```

![]()


## Conclusion
### Unmet requirements: 

