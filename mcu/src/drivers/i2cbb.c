/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * i2c BIT BANG driver
 * 
 * created by rdupu13
 * 
 * this file does NOT use the eUSCI peripheral
 * @file i2cbb.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "drivers/i2cbb.h"

// hardware
#include "hw/pfc.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

unsigned char *i2cbb_tx_buf_ptr;
unsigned char *i2cbb_rx_buf_ptr;

unsigned char i2cbb_mode;
unsigned char i2cbb_byte;
unsigned char i2cbb_sda_rx;
unsigned char i2cbb_nack;
unsigned char i2cbb_end_rx;
unsigned char i2cbb_slave_addr;
unsigned char i2cbb_busy;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

void i2cbb_busy_set(void);
void i2cbb_busy_clear(void);

void i2cbb_tx_start(void);
void i2cbb_tx_stop(void);
void i2cbb_tx_byte(void);
void i2cbb_rx_byte(void);

/**
 * @brief initialize i2c bit banger
 * 
 * @return none
 */
void i2cbb_init(void)
{
    // configure pins
    I2C_SEL0 &= ~I2C_PINS;
    I2CBB_PORT |= (I2CBB_SCL | I2CBB_SDA);
    
    // initialize variables
    i2cbb_tx_buf_ptr = 0;
    i2cbb_rx_buf_ptr = 0;
    i2cbb_mode = 0;
    i2cbb_byte = 0;
    i2cbb_sda_rx = 0;
    i2cbb_nack = 0;
    i2cbb_end_rx = 0;
    i2cbb_slave_addr = 0;
    i2cbb_busy_clear();
}

/*
/**
 * @brief write an array to an i2c slave
 * 
 * @param arr           pointer to array to be written
 * @param len           length in bytes of array to be written
 * @param slave_addr    slave address
 * @param reg_addr      slave register address to start writing at
 * 
 * @return status of write:
 *           0: ok
 *          -1: bus busy
 *          -2: 0 length error
 *          -4: nack error
 */
int i2cbb_write(
    unsigned char *arr,
    unsigned int len,
    unsigned int slave_addr,
    unsigned char reg_addr
) {
    if (i2cbb_busy) { return -1; }
    if (len == 0) { return -2; }

    i2cbb_tx_buf_ptr = arr;
    i2cbb_slave_addr = slave_addr;
    
    i2cbb_busy_set();

    i2cbb_mode = 0; // write mode
    i2cbb_tx_start();

    i2cbb_byte = reg_addr;
    i2cbb_tx_byte();

    unsigned char i;
    for (i = 0; i < len; i++)
    {
        i2cbb_byte = *i2cbb_tx_buf_ptr++;
        i2cbb_tx_byte();

        if (i2cbb_nack) {
            i2cbb_busy_clear();
            return -4;
        }
    }

    i2cbb_tx_stop();

    i2cbb_busy_clear();

    return 0;
}

/**
 * @brief read an array from an i2c slave
 * 
 * @param arr           pointer to array to store received data
 * @param len           length in bytes of array to be read
 * @param slave_addr    slave address
 * @param reg_addr      slave register address to start reading from
 * 
 * @return status of read:
 *           0: ok
 *          -1: bus busy
 *          -2: 0 length error
 *          -4: nack error
 */
int i2cbb_read(
    unsigned char *arr,
    unsigned int len,
    unsigned int slave_addr,
    unsigned char reg_addr
) {
    if (i2cbb_busy) { return -1; }
    if (len == 0) { return -2; }

    i2cbb_rx_buf_ptr = arr;
    i2cbb_slave_addr = slave_addr;

    i2cbb_mode = 1; // read mode

    i2cbb_busy_set();
    
    i2cbb_mode = 0;
    i2cbb_tx_start();

    i2cbb_byte = reg_addr;
    i2cbb_tx_byte();

    if (i2cbb_nack) {
        i2c_busy_clear();
        return -4;
    }

    i2cbb_mode = 1;
    i2cbb_tx_start();

    unsigned char i;
    for (i = 0; i < len; i++)
    {
        if (i == len - 1) {
            i2cbb_end_rx = 1;
        }

        i2cbb_rx_byte();
        *i2cbb_rx_buf_ptr++ = i2cbb_byte;
    }

    i2cbb_tx_stop();
    i2cbb_busy_clear();

    return 0;
}

/**
 * 
 */
void i2cbb_tx_start(void)
{
    i2cbb_nack = 0;

    I2CBB_PORT |= (I2CBB_SCL | I2CBB_SDA);
    I2CBB_DELAY

    I2CBB_PORT &= ~I2CBB_SDA;
    I2CBB_DELAY
    I2CBB_PORT &= ~I2CBB_SCL;
    I2CBB_DELAY

    i2cbb_byte = ((i2cbb_slave_addr << 1) | i2cbb_mode);
    i2cbb_tx_byte();
}

/**
 * 
 */
void i2cbb_tx_stop(void)
{
    if (i2cbb_nack) { return; }

    I2CBB_PORT &= ~I2CBB_SDA;
    I2CBB_DELAY
    I2CBB_PORT |= I2CBB_SCL;
    I2CBB_DELAY
    I2CBB_PORT |= I2CBB_SDA;
    I2CBB_DELAY
}

/**
 * 
 */
void i2cbb_tx_byte(void)
{
    if (i2cbb_nack) { return; }

    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        if (i2cbb_byte & 0x80) {
            I2CBB_PORT |= I2CBB_SDA;
        } else {
            I2CBB_PORT &= ~I2CBB_SDA;
        }

        I2CBB_DELAY
        I2CBB_PORT |= I2CBB_SCL;
        I2CBB_DELAY
        I2CBB_DELAY
        I2CBB_DELAY
        I2CBB_PORT &= ~I2CBB_SCL;
        I2CBB_DELAY

        i2cbb_byte = i2cbb_byte << 1;
    }

    I2CBB_DIR &= ~I2CBB_SDA;
    I2CBB_PORT |= I2CBB_SDA;

    I2CBB_DELAY
    I2CBB_PORT |= I2CBB_SCL;
    I2CBB_DELAY

    i2cbb_sda_rx = (I2CBB_IN & I2CBB_SDA);

    I2CBB_DELAY
    I2CBB_PORT &= ~I2CBB_SCL;
    I2CBB_DELAY
    
    I2CBB_DIR |= I2CBB_SDA;
    I2CBB_PORT |= I2CBB_SDA;

    if (i2cbb_sda_rx) {
        i2cbb_tx_stop();
        i2cbb_nack = 1;
    }
}

/**
 * 
 */
void i2cbb_rx_byte(void)
{
    if (i2cbb_nack) { return; }

    I2CBB_DIR &= ~I2CBB_SDA;

    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        I2CBB_DELAY
        I2CBB_PORT |= I2CBB_SCL;
        I2CBB_DELAY

        i2cbb_sda_rx = (I2CBB_IN & I2CBB_SDA);

        I2CBB_DELAY
        I2CBB_PORT &= ~I2CBB_SCL;
        I2CBB_DELAY

        i2cbb_byte = i2cbb_byte << 1;
        if (i2cbb_sda_rx) {
            i2cbb_byte |= 1;
        }
    }

    I2CBB_DIR |= I2CBB_SDA;
    I2CBB_PORT |= I2CBB_SDA;

    if (i2cbb_end_rx) {
        I2CBB_PORT |= I2CBB_SDA;
        i2cbb_end_rx = 0;
    } else {
        I2CBB_PORT &= ~I2CBB_SDA;
    }

    I2CBB_DELAY
    I2CBB_PORT |= I2CBB_SCL;
    I2CBB_DELAY
    I2CBB_DELAY
    I2CBB_DELAY
    I2CBB_PORT &= ~I2CBB_SCL;
    I2CBB_DELAY
}

/**
 * @brief set the busy flag
 * 
 * @return none
 */
void i2cbb_busy_set(void)
{
    I2CBB_BUSY_PORT |= I2CBB_BUSY_PIN;
    i2cbb_busy = 1;
}

/**
 * @brief clear the busy flag
 * 
 * @return none
 */
void i2cbb_busy_clear(void)
{
    I2CBB_BUSY_PORT &= ~I2CBB_BUSY_PIN;
    i2cbb_busy = 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
