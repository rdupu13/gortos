/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * input output
 * 
 * created by rdupu13
 * 
 * @file gio.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include <stdint.h>

#include "kernel/gio.h"

// kernel
#include "kernel/gsys.h"
#include "kernel/gstr.h"

// drivers
#include "drivers/uart.h"
#include "drivers/i2c.h"
#include "drivers/spi.h"
#include "drivers/led.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief write an array to gout
 * 
 * @param arr array to be written
 * 
 * @return status of write
 */
int helloworld(char *arr)
{
    int stat = 0;
    switch(gout)
    {
        case 0:
            // uart
            uart_tx(girth(arr), (uint8_t *) arr);
            break;
        
        default:
            stat = 1; // raise error
            break;
    }
    return stat;
}

/**
 * @brief read an array from gin
 * 
 * @param arr   pointer to where received array will be stored
 * @param n     number of bytes to read (> 0)
 * @param stop  if n = 0, read until this character received
 * 
 * @return status of read
 */
int hellogort(char *arr, int n, char stop)
{
    int ret = 0;
    switch(gin)
    {
        case 0:
            // uart
            uart_rx((uint16_t) n, (uint8_t *) arr, (uint8_t) stop);
            break;
        
        default: break;
    }
    return ret;
}

/**
 * @brief write clear sequence to gout
 * 
 * @param none
 * 
 * @return none
 */
void glear()
{
    char seq[] = PUTTY_CLEAR_SEQ;
    helloworld(seq);
}

/**
 * blink the TEST0 LED for delay ms
 */
void blinky(unsigned int delay)
{
    LED_TEST0_PORT |= LED_TEST0_PIN;
    eep(delay);
    LED_TEST0_PORT &= ~LED_TEST0_PIN;
}

//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
