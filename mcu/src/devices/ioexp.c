/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * io expander driver
 * 
 * created by rdupu13
 * 
 * @file ioexp.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "devices/ioexp.h"

// drivers
#include "drivers/i2c.h"

// kernel
#include "kernel/gsys.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize io expander
 * 
 * @return status of init:
 *           0: ok
 *          -1: 
 */
int ioexp_init(void)
{
    //unsigned char client_addr = 0x00;
    unsigned char reg_config1[] = {
        0x00,
        0x00, 0x00, 0x00, 0x00,
        0b00100100,
        0x00
    };

    unsigned char gpio = 0x00;

    // TODO: idek what this register does
    unsigned char olat = 0x00;

    int stat = i2c_write(
        reg_config1,
        7,
        IOEXP_CLIENT_ADDR,
        IOEXP_REG_IODIR
    );
    if (ioexp_errmsg(stat)) { return stat; }
    
    stat = i2c_write(
        &gpio,
        1,
        IOEXP_CLIENT_ADDR,
        IOEXP_REG_GPIO
    );
    if (ioexp_errmsg(stat)) { return stat; }

    stat = i2c_write(
        &olat,
        1,
        IOEXP_CLIENT_ADDR,
        IOEXP_REG_OLAT
    );
    if (ioexp_errmsg(stat)) { return stat; }
    
    return 0;
}

/**
 * @brief 
 * 
 * @param gpio 
 * 
 * @return status of write:
 *           0: ok
 *          -1: 
 */
int ioexp_write(unsigned char gpio)
{
    int stat = i2c_write(
        &gpio,
        1,
        IOEXP_CLIENT_ADDR,
        IOEXP_REG_GPIO
    );
    if (ioexp_errmsg(stat)) { return stat; }
    
    return 0;
}

/**
 * @brief 
 * 
 * @param 
 * 
 * @return none
 */
int ioexp_errmsg(int stat)
{
    switch (stat)
    {
        case -1: gsys_log("ioexp: i2c bus busy"); break;
        case -2: gsys_log("ioexp: i2c 0 length error"); break;
        case -3: gsys_log("ioexp: i2c timeout error"); break;
        case -4: gsys_log("ioexp: i2c NACK received error"); break;
        default: break;
    }
    return stat;
}


//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
