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

// hardware
#include "hw/pfc.h"


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

/*
#define I2C_CTLW0 // control word 0
#define I2C_BRW // baud rate word
#define I2C_IFG // interrupt flags
#define I2C_IE // interrupt enable
#define I2C_IV // interrupt vector
#define I2C_SA // slave address
#define I2C_TXBUF // tx buffer
#define I2C_RXBUF // rx buffer

#define I2C_SWRST // software reset
#define I2C_SSEL // clock source select
#define I2C_MODE // peripheral mode
#define I2C_MST // master mode
#define I2C_A10 // 10-bit/7-bit addressing
#define I2C_SLA10 // 10-bit/7-bit slave addressing
#define I2C_NACKIFG // no acknowledge interrupt flag
#define I2C_NACKIE // no acknowledge interrupt enable
#define I2C_TXIFG // tx complete interrupt flag
#define I2C_TXIE // tx complete interrupt enable
#define I2C_RXIFG // rx buffer full interrupt flag
#define I2C_RXIE // rx buffer full interrupt enable
#define I2C_UCTR // tx/rx mode
#define I2C_TXSTT // tx start condition + slave address
#define I2C_TXSTP // tx stop condition
*/

#define I2C_BUSY_PORT   LED_TEST1_PORT
#define I2C_BUSY_PIN    LED_TEST1_PIN


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

void i2c_init(unsigned int timeout); // initialize i2c

extern volatile unsigned int i2c_arblost;

// write an array to an i2c slave
int i2c_write(
    volatile unsigned char *arr,
    unsigned int len,
    unsigned int slave_addr,
    unsigned char reg_addr
);

// read an array from an i2c slave 
int i2c_read(
    volatile unsigned char *arr,
    unsigned int len,
    unsigned int slave_addr,
    unsigned char reg_addr
);


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
