/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * i2c driver library
 * 
 * created by rdupu13
 * 
 * @file i2c.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef I2C_H
#define I2C_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include <msp430fr2153.h>

// drivers
#include "drivers/led.h"


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

#define I2C_SEL0        P4SEL0
#define I2C_PINS        BIT6 | BIT7

#define I2C_BUSY_PORT   LED_TEST1_PORT
#define I2C_BUSY_PIN    LED_TEST1_PIN


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

void i2c_init(void); // initialize i2c

// write an array to an i2c slave
void i2c_write(
    unsigned int slave_addr,
    unsigned char reg_addr,
    unsigned int len,
    unsigned char *arr
);

// read an array from an i2c slave 
void i2c_read(
    unsigned int slave_addr,
    unsigned char reg_addr,
    unsigned int len,
    unsigned char *arr
);


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
