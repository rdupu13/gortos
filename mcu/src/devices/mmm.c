/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * mini memory module driver
 * 
 * created by rdupu13
 * 
 * @file mmm.c
 *
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "devices/mmm.h"

// drivers
#include "drivers/spi.h"

// kernel
#include "kernel/gsys.h"
#include "kernel/gstr.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char mmm_instr[4];

unsigned char mmm_initialized;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

void get_mmm_addr(unsigned int blk_num);

/**
 * @brief initialize mini memory manager
 * 
 * @return status of initialization via spi:
 *           0: ok
 *          -1: bus busy
 *          -2: 0 length error
 *          -3: timeout error
 */
int mmm_init(void)
{
    volatile unsigned char mode_reg;
    
    mmm_instr[0] = 0x05; // read mode register (RDMR)
    
    mmm_instr[1] = 0x00; // address MSB
    mmm_instr[2] = 0xAB;
    mmm_instr[3] = 0xCD; // address LSB
    
    mmm_initialized = 1;

    int stat = spi_read(
        MMM_SPI_SLAVE_NUM,
        mmm_instr,
        1,
        &mode_reg,
        1
    );
    if (stat) { mmm_initialized = 0; return stat; }

    gsys_log("mmm: mode register:");
    gsys_log(hex((unsigned int) mode_reg));

    return 0;
}

/**
 * @brief load a block from memory
 * 
 * @param blk block object to load
 * 
 * @return status of load operation:
 *           0: ok
 *          -1: bus busy
 *          -2: 0 length error
 *          -3: timeout error
 *          -4: mmm not initialized error
 */
int mmm_load_block(gblk_t *blk)
{
    if (!mmm_initialized) { return -4; }

    mmm_instr[0] = 0x03;

    get_mmm_addr(blk->num);
    
    int stat = spi_read(
        MMM_SPI_SLAVE_NUM,
        mmm_instr,
        4,
        blk->data,
        BLK_SIZE
    );
    if (stat) { return stat; }

    return 0;
}

/**
 * @brief store a block to memory
 * 
 * @param blk block object to store
 * 
 * @return status of store operation:
 *           0: ok
 *          -1: bus busy
 *          -2: 0 length error
 *          -3: timeout error
 *          -4: mmm not initialized error
 */
int mmm_store_block(gblk_t *blk)
{
    if (!mmm_initialized) { return -4; }

    mmm_instr[0] = 0x02; // WRITE

    get_mmm_addr(blk->num);
    
    int stat = spi_write(
        MMM_SPI_SLAVE_NUM,
        mmm_instr,
        4,
        blk->data,
        BLK_SIZE
    );
    if (stat) { return stat; }

    return 0;
}

/**
 * @brief get the mmm address of a block object
 * 
 * @param blk_num block object number
 * 
 * @return none
 */
void get_mmm_addr(unsigned int blk_num)
{
    mmm_instr[1] = 0; // TODO: handle someday

    // TODO: assumes BLK_SIZE is always 256
    mmm_instr[2] = blk_num; // block address
    mmm_instr[3] = 0; // byte address, always block-aligned
}

//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
