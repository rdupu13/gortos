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

// TODO: make these files:
int gin;
int gout;


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
                gstr_len(arr)
            );
            break;
        
        case 1:
            // lcd
            lcd_write(arr);
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
int glear(void)
{
    int ret;
    switch(gout)
    {
        case 0:
            ret = helloworld(PUTTY_CLEAR_SEQ);
            break;
        
        case 1:
            ret = lcd_clear();
            if (ret) { return ret; }
            ret = lcd_cursor_goto(0, 0);
            break;
        
        default: ret = -1; break;
    }
    return ret;
}

/**
 * 
 */
int ghome(void)
{
    int ret;
    switch(gout)
    {
        case 0:
            ret = helloworld("\r");
            break;
        
        case 1:
            ret = lcd_cursor_goto(0, 0);
            break;
        
        default: ret = -1; break;
    }
    return ret;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
