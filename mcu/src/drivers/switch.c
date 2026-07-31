/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * switch driver
 * 
 * created by rdupu13
 * 
 * @file switch.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "drivers/switch.h"

// hardware
#include "hw/pfc.h"

// kernel
#include "kernel/gsys.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize switches
 * 
 * @return none
 */
void switch_init(void)
{
    // initialize switch 0
    SWITCH0_DIR &= ~SWITCH0_PIN; // set as input
    SWITCH0_REN |=  SWITCH0_PIN; // enable resistor
    SWITCH0_OUT |=  SWITCH0_PIN; // pull-up resistor
    if (SWITCH_INTERRUPT_EN) {
        SWITCH0_IES |=  SWITCH0_PIN; // falling-edge interrupts
        SWITCH0_IE  |=  SWITCH0_PIN; // enable interrupts
        SWITCH0_IFG &= ~SWITCH0_PIN; // clear interrupt flag
    }

    // initialize switch 1
    SWITCH1_DIR &= ~SWITCH1_PIN; // set as input
    SWITCH1_REN |=  SWITCH1_PIN; // enable resistor
    SWITCH1_OUT |=  SWITCH1_PIN; // pull-up resistor
    if (SWITCH_INTERRUPT_EN) {
        SWITCH1_IES |=  SWITCH1_PIN; // falling-edge interrupts
        SWITCH1_IE  |=  SWITCH1_PIN; // enable interrupts
        SWITCH1_IFG &= ~SWITCH1_PIN; // clear interrupt flag
    }
}

/**
 * 
 */
unsigned char switch_poll(unsigned char n)
{
    unsigned char res;
    switch(n)
    {
        case 0: res = (SWITCH0_IN & SWITCH0_PIN) ? 0 : 1; break;
        case 1: res = (SWITCH1_IN & SWITCH1_PIN) ? 0 : 1; break;
        default: res = 0; break; 
    }
    return res;
}

//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

#pragma vector = SWITCH_PORT_VECTOR
__interrupt void isr_port(void)
{
    switch(SWITCH_IV)
    {
        case SWITCH0_IV_VAL: switch_0_pressed(); break;
        case SWITCH1_IV_VAL: switch_1_pressed(); break;
        
        /* TODO: attempt to alleviate stupid switch debounce
        case SWITCH0_IV_VAL:
            SWITCH0_IE  &=  ~SWITCH0_PIN; // enable interrupts
            break;
        
        case SWITCH1_IV_VAL:
            SWITCH0_IE  &=  ~SWITCH0_PIN; // enable interrupts
            break;
        */

        default: break;
    }
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
