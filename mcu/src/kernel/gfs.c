/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * gort file system
 * 
 * created by rdupu13
 * 
 * @file gfs.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "kernel/gfs.h"

// drivers
#include "drivers/uart.h"

// devices
#include "devices/dial.h"
#include "devices/mmm.h"
#include "devices/lcd.h"
#include "devices/lora.h"

// kernel
#include "kernel/gstr.h"
#include "kernel/gsys.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

gfile_t uart;
//gfile_t lcd;
//gfile_t dial;
//gfile_t lora;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * 
 */
void gfs_init(void)
{
    // uart ---------------------------
    uart.mode = 0b00011010;
    uart.size = 0;
    uart.loaded = 1;
    gstr_copy(uart.name, "uart", 5);
    uart.blk = 0;
    // --------------------------------

    /*
    // lcd ----------------------------
    lcd.mode = 0b00101010;
    lcd.size = 0;
    lcd.loaded = 1;
    gopy(lcd.name, "lcd", 5);
    lcd.blk = 0;
    // --------------------------------

    // dial ---------------------------
    dial.mode = 0b01010010;
    dial.size = 0;
    dial.loaded = 1;
    gopy(dial.name, "dial", 5);
    dial.blk = 0;
    // --------------------------------

    // lora ---------------------------
    lora.mode = 0b01111010;
    lora.size = 0;
    lora.loaded = 1;
    gopy(lora.name, "lora", 5);
    lora.blk = 0;
    // --------------------------------
    */

}

/**
 * @brief open a file
 * 
 * @param path path to file
 * 
 * @return pointer to opened file
 */
gfile_t *file_open(char *path)
{
    
}

/**
 * @brief write to a file
 * 
 * @param fp pointer to file
 * @param arr array to write
 * 
 * @return status of write:
 *              0: ok
 *              
 */
int file_write(gfile_t *fp, char *arr)
{
    return 0;
}

/**
 * @brief read from a file
 * 
 * @param fp pointer to file
 * @param arr array to store read data
 * @param len number of bytes to read (len > 0)
 * @param stop if len = 0, read until this char
 * 
 * @return status of read:
 *              0: ok
 *              
 */
int file_read(gfile_t *fp, char *arr, unsigned int len, char stop)
{
    return 0;
}

/**
 * @brief close a file
 * 
 * @param fp pointer to file
 * 
 * @return none
 */
void file_close(gfile_t *fp)
{

}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
