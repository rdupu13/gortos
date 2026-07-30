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

// devices
#include "devices/uart.h"

// kernel
#include "kernel/gstr.h"


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
    int status = 0;
    switch(gout)
    {
        case 0:
            // uart
            uart_tx(
                girth(arr),
                (unsigned char *) arr
            );
            break;
        
        default:
            status = 1; // raise error
            break;
    }
    return status;
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
    int status = 0;
    switch(gin)
    {
        case 0:
            // uart
            uart_rx(
                (unsigned int) n,
                (unsigned char *) arr,
                (unsigned char) stop
            );
            break;
        
        default:
            status = 1;
            break;
    }
    return status;
}

/**
 * @brief write clear sequence to gout
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
