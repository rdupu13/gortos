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

// drivers
#include "drivers/uart.h"
#include "drivers/i2c.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

// TODO: make GOUT and GIN variable

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
    switch(GOUT)
    {
        case 0:
            // uart
            uart_tx((uint8_t *) arr);
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
char *hellogort(int n, char stop)
{
    char *ret = 0;
    switch(GIN)
    {
        case 0:
            // uart
            ret = (char *) uart_rx((uint8_t) n, (uint8_t) stop);
            break;
        
        case 1:
            // i2c
            // TODO: check n
            ret = (char *) i2c_read(
                0x0068, 
                0x00, 
                (uint8_t) n
            );
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
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
