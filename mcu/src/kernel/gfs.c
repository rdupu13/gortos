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

// devices
#include "devices/mmm.h"

// kernel
#include "kernel/gstr.h"
#include "kernel/gsys.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * 
 */
void gfs_init(void)
{
    

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
