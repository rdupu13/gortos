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
void mmm_init(void)
{
    volatile unsigned char instr;
    volatile unsigned char mode_reg;
    
    instr = 0x05; // read mode register
    
    // TODO: handle errors
    //SPI_CS0_PORT &= ~SPI_CS0_PIN;
    spi_read(&mode_reg, 1, MMM_SPI_SLAVE_NUM, &instr, 1);
    
    mode_reg &= ~0xC0;              // clear mode
    mode_reg |= (MMM_MODE << 6);    // set mode

    instr = 0x01; // write mode register
    volatile unsigned char wr[] = {0x01, 0x80};

    // TODO: handle errors
    spi_write(wr, 2, MMM_SPI_SLAVE_NUM);
    //spi_stop();
    //SPI_CS0_PORT |= SPI_CS0_PIN;
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
int mmm_load_block(gblk_t *blk)
{
    volatile unsigned char instr[4] = {
        0x03, mmm_addr[2], mmm_addr[1], mmm_addr[0]
    };

    get_mmm_addr(blk->num);
    
    // TODO: handle errors
    spi_read(blk->data, BLK_SIZE, MMM_SPI_SLAVE_NUM, instr, 4);

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
int mmm_store_block(gblk_t *blk)
{
    volatile unsigned char instr[4] = {
        0x02, mmm_addr[2], mmm_addr[1], mmm_addr[0]
    };

    get_mmm_addr(blk->num);
    
    spi_write(instr, 4, MMM_SPI_SLAVE_NUM);
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
