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
#include "drivers/led.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile char gin_buf[GIN_BUF_SIZE];


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
        
        case 1:
            // i2c
            i2c_write(
                0x0068,
                0x00,
                (uint8_t) girth(arr),
                (uint8_t *) arr
            );
            break;

        case 2:
            // spi
            spi_write(
                0x1E,
                (uint16_t) girth(arr),
                (uint8_t *) arr
            );
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
 * @param n     number of bytes to read (> 0)
 * @param stop  if n = 0, read until this character received
 * 
 * @return pointer to received array, null if unsuccessful
 */
int hellogort(char *arr, int n, char stop)
{
    int ret = 0;
    switch(gin)
    {
        case 0:
            // uart
            uart_rx((uint16_t) n, (uint8_t *) gin_buf, (uint8_t) stop);
            break;
        
        case 1:
            /* i2c
            ret = (char *) i2c_read(
                0x0068, 
                0x00, 
                (uint8_t) n
            );*/
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
 * blink the TEST1 LED for delay ms
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
