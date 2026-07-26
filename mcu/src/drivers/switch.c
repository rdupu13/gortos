#include <msp430fr2153.h>
#include <stdint.h>

#include "switch.h"
#include "led.h"

/**
 * 
 */
void switch_init()
{
    // initialize switch 0
    SWITCH0_DIR &= ~SWITCH0_PIN; // set as input
    SWITCH0_REN |=  SWITCH0_PIN; // enable resistor
    SWITCH0_OUT |=  SWITCH0_PIN; // pull-up resistor
    SWITCH0_IES |=  SWITCH0_PIN; // falling-edge interrupts
    SWITCH0_IE  |=  SWITCH0_PIN; // enable interrupts
    SWITCH0_IFG &= ~SWITCH0_PIN; // clear interrupt flag

    // initialize switch 1
    SWITCH1_DIR &= ~SWITCH1_PIN; // set as input
    SWITCH1_REN |=  SWITCH1_PIN; // enable resistor
    SWITCH1_OUT |=  SWITCH1_PIN; // pull-up resistor
    SWITCH1_IES |=  SWITCH1_PIN; // falling-edge interrupts
    SWITCH1_IE  |=  SWITCH1_PIN; // enable interrupts
    SWITCH1_IFG &= ~SWITCH1_PIN; // clear interrupt flag
}

#pragma vector = PORT5_VECTOR
__interrupt void port5_isr(void)
{
    LED_TEST0_PORT ^= LED_TEST0_PIN;

    SWITCH0_IFG &= ~SWITCH0_PIN;
    SWITCH1_IFG &= ~SWITCH1_PIN;
}