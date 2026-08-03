/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * mini memory manager
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


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char mmm_instr;
volatile unsigned char mmm_addr[3];


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

void get_mmm_addr(unsigned int blk_num);

/**
 * @brief initialize mini memory manager
 * 
 * @return none
 */
// TODO: add to gsys_init
void mmm_init(void)
{
    volatile unsigned char mode_reg;
    // TODO: handle errors
    spi_read(&mode_reg, 1, MMM_SPI_SLAVE_NUM);
    mode_reg &= ~0xC0;
    mode_reg |= (MMM_MODE << 6);
    spi_write(&mode_reg, 1, MMM_SPI_SLAVE_NUM);
}

/**
 * @brief load a block from memory
 * 
 * @param blk 
 * 
 * @return status of load:
 *              0: ok
 *              
 */
int mmm_load_block(blk_t *blk)
{
    get_mmm_addr(blk->n);
    // TODO: check if transaction actually stays live across multiple writes
    // TODO: handle errors
    spi_write(&mmm_instr, 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[2], 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[1], 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[0], 1, MMM_SPI_SLAVE_NUM);
    spi_read(blk->data, BLK_SIZE, MMM_SPI_SLAVE_NUM);
}

/**
 * @brief store a block to memory
 * 
 * @param blk
 * 
 * @return status of load:
 *              0: ok
 *              
 */
int mmm_store_block(blk_t *blk)
{
    get_mmm_addr(blk->n);
    // TODO: check if transaction actually stays live across multiple writes
    // TODO: handle errors
    spi_write(&mmm_instr, 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[2], 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[1], 1, MMM_SPI_SLAVE_NUM);
    spi_write(&mmm_addr[0], 1, MMM_SPI_SLAVE_NUM);
    spi_write(blk->data, BLK_SIZE, MMM_SPI_SLAVE_NUM);
}

/**
 * 
 */
void get_mmm_addr(unsigned int blk_num)
{
    mmm_addr[2] = 0; // TODO: handle someday
    mmm_addr[1] = (blk_num << BLK_SIZE_P2); // block address
    mmm_addr[0] = 0; // byte address
}

//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
