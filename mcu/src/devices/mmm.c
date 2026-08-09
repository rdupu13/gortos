/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * mini memory module driver
 * 
 * created by rdupu13
 * 
 * @file mmm.c
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


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char mmm_addr[3];


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

void get_mmm_addr(unsigned int blk_num);

// TODO: check if spi transactions actually stay live
// across multiple read/write calls

/**
 * @brief initialize mini memory manager
 * 
 * @return none
 */
// TODO: add to gsys_init
void mmm_init(void)
{
    volatile unsigned char instr;
    volatile unsigned char mode_reg;
    
    instr = 0x05; // read mode register
    
    // TODO: handle errors
    spi_write(&instr, 1, MMM_SPI_SLAVE_NUM);
    spi_read(&mode_reg, 1, MMM_SPI_SLAVE_NUM);
    
    mode_reg &= ~0xC0;
    mode_reg |= (MMM_MODE << 6); // set mode

    instr = 0x01; // write mode register

    // TODO: handle errors
    spi_write(&instr, 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mode_reg, 1, MMM_SPI_SLAVE_NUM);
}

/**
 * @brief load a block from memory
 * 
 * @param blk block object to load
 * 
 * @return status of load operation:
 *              0: ok
 *              
 */
int mmm_load_block(blk_t *blk)
{
    volatile unsigned char instr;

    instr = 0x03; // read
    get_mmm_addr(blk->num);
    
    // TODO: handle errors
    spi_write(&instr, 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[2], 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[1], 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[0], 1, MMM_SPI_SLAVE_NUM);

    spi_read(blk->data, BLK_SIZE, MMM_SPI_SLAVE_NUM);

    return 0;
}

/**
 * @brief store a block to memory
 * 
 * @param blk block object to store
 * 
 * @return status of store operation:
 *              0: ok
 *              
 */
int mmm_store_block(blk_t *blk)
{
    volatile unsigned char instr;

    instr = 0x02; // write
    get_mmm_addr(blk->num);
    
    // TODO: handle errors
    spi_write(&instr, 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[2], 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[1], 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[0], 1, MMM_SPI_SLAVE_NUM);

    spi_write(blk->data, BLK_SIZE, MMM_SPI_SLAVE_NUM);

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
    mmm_addr[2] = 0; // TODO: handle someday

    // TODO: assumes BLK_SIZE is always 256
    mmm_addr[1] = blk_num; // block address
    mmm_addr[0] = 0; // byte address, always block-aligned
}

//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
