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

#include "kernel/gio.h"

// drivers
#include "drivers/uart.h"

// kernel
#include "kernel/gstr.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief write a null-terminated array to gout
 * 
 * @param arr array to be written
 * 
 * @return status of write:
 *               0: ok
 *              -1: error
 */
int helloworld(char *arr)
{
    int status = 0;
    switch(gout)
    {
        case 0:
            // uart
            // TODO: handle errors
            uart_tx(
                (volatile unsigned char *) arr,
                girth(arr)
            );
            break;
        
        default:
            status = -1; // raise error
            break;
    }
    return status;
}

/**
 * @brief read an array from gin
 * 
 * @param arr   pointer to where received array will be stored
 * @param len   number of bytes to read (> 0)
 * @param stop  if n = 0, read until this character received
 * 
 * @return status of read:
 *               0: ok
 *              -1: error
 */
int hellogort(char *arr, unsigned int len, char stop)
{
    int status = 0;
    switch(gin)
    {
        case 0:
            // uart
            // TODO: handle errors
            uart_rx(
                (volatile unsigned char *) arr,
                len,
                (unsigned char) stop
            );
            break;
        
        default:
            status = -1;
            break;
    }
    return status;
}

/**
 * @brief write putty clear sequence to gout
 * 
 * @param none
 * 
 * @return none
 */
void glear(void)
{
    char seq[] = PUTTY_CLEAR_SEQ;
    helloworld(seq);
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
