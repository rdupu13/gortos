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

unsigned char switch_interrupt_en;
unsigned char switch_pol;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize switches
 * 
 * @param pol0 switch 0 polarity
 * @param pol1 switch 1 polarity
 * 
 * @return none
 */
void switch_init(
    unsigned char pol0,
    unsigned char pol1
) {
    // create bools of switch polarity
    switch_pol = pol0;
    switch_pol |= (pol1 << 1);

    // initialize switch 0 --------------------------------
    SWITCH0_DIR &= ~SWITCH0_PIN; // set as input
    SWITCH0_REN |=  SWITCH0_PIN; // enable resistor
    if (pol0) {
        SWITCH0_OUT &= ~SWITCH0_PIN; // pull-down resistor // TODO: should these even be with IES? maybe hardcoded bc hardware
        SWITCH0_IES &= ~SWITCH0_PIN; // rising-edge interrupts
    } else {
        SWITCH0_OUT |= SWITCH0_PIN; // pull-up resistor
        SWITCH0_IES |= SWITCH0_PIN; // falling-edge interrupts
    }
    // ----------------------------------------------------

    // initialize switch 1 --------------------------------
    SWITCH1_DIR &= ~SWITCH1_PIN; // set as input
    SWITCH1_REN |= SWITCH1_PIN; // enable resistor
    if (pol1) {
        SWITCH1_OUT &= ~SWITCH1_PIN; // pull-down resistor
        SWITCH1_IES &= ~SWITCH1_PIN; // rising-edge interrupts
    } else {
        SWITCH1_OUT |= SWITCH1_PIN; // pull-up resistor
        SWITCH1_IES |= SWITCH1_PIN; // falling-edge interrupts
    }
    // ----------------------------------------------------
    
    if (SWITCH_INTERRUPT_INIT_EN) {
        SWITCH0_IFG &= ~SWITCH0_PIN; // clear interrupt flag
        SWITCH0_IE  |=  SWITCH0_PIN; // enable interrupts

        SWITCH1_IFG &= ~SWITCH1_PIN; // clear interrupt flag
        SWITCH1_IE  |=  SWITCH1_PIN; // enable interrupts
    }
}

/**
 * @brief poll a switch input
 * 
 * @param n switch number
 * 
 * @return value of the switch input
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

/**
 * @brief wait for a switch to be pressed/unpressed
 * 
 * @param n switch number
 * @param lvl switch level
 *      0 = wait to be unpressed
 *      1 = wait to be pressed
 * 
 * @return none
 */
void switch_wait(unsigned char n, unsigned char lvl)
{
    while (
        switch_poll(n) // irl high/low value of input
        != 
        (((switch_pol >> n) & 1) // value of switch polarity TODO: boolean byte = bb?
            ^ lvl) // 
    ) {}
}

//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

void __attribute__((interrupt(SWITCH_VECTOR))) isr_port(void)
{
    switch(SWITCH_IV)
    {
        case SWITCH0_IV_VAL: switch_0_pressed(); break;
        case SWITCH1_IV_VAL: switch_1_pressed(); break;
        
        /* TODO: attempt to alleviate stupid switch debounce
        case SWITCH0_IV_VAL:
            SWITCH0_IE  &=  ~SWITCH0_PIN; // disable interrupts
            break;
        
        case SWITCH1_IV_VAL:
            SWITCH0_IE  &=  ~SWITCH0_PIN; // disable interrupts
            break;
        */

        default: break;
    }
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
