/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * lcd driver
 * 
 * created by rdupu13
 * 
 * @file lcd.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "devices/lcd.h"

// devices
#include "devices/ioexp.h"

// kernel
#include "kernel/gsys.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

unsigned char lcd_reg;



//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

int lcd_write_nibble(
    unsigned char nibble,
    unsigned char reg_sel
);

int lcd_write_byte(
    unsigned char byte,
    unsigned char reg_sel
);



/**
 * @brief initialize lcd
 * 
 * @return none
 */
int lcd_init(void)
{
    eep(50);
    lcd_write_nibble(0x03, 0);
    eep(5);
    lcd_write_nibble(0x03, 0);
    eep(1);
    lcd_write_nibble(0x03, 0);
    eep(1);
    lcd_write_nibble(0x02, 0);
    eep(1);

    lcd_write_byte(0x28, 0); // 4-bit, 2 lines, 5x8 font
    lcd_write_byte(0x06, 0); // incerement cursor, no shift
    lcd_write_byte(0x0F, 0); // display on, cursor + blink on
    lcd_write_byte(0x01, 0); // clear display
    eep(5);

    return 0;
}

/**
 * @brief
 * 
 * @param nibble
 * @param reg_sel
 * 
 * @return 
 */
int lcd_write_nibble(
    unsigned char nibble,
    unsigned char reg_sel
) {
    unsigned char base = \
        ((nibble & 0x0F) << 4) | (reg_sel ? LCD_RS : 0);
    
    ioexp_write(base);
    ioexp_write(base | LCD_EN);
    ioexp_write(base);
    return 0;
}

/**
 * @brief
 * 
 * @param byte
 * @param reg_sel
 * 
 * @return 
 */
int lcd_write_byte(
    unsigned char byte,
    unsigned char reg_sel
) {
    if (reg_sel) {
        switch (byte)
        {
            case '\n':
                // some dumb shit
                break;
            default: break;
        }
    } else {
        lcd_write_nibble(byte >> 4, reg_sel);
        lcd_write_nibble(byte & 0x0F, reg_sel);
    }
    return 0;
}

/**
 * @brief print a character to the lcd display
 * 
 * @param 
 * 
 * @return none
 */
int lcd_write(
    unsigned char *arr
) {
    unsigned int i;
    for (i = 0; arr[i] != '\0'; i++)
    {
        lcd_write_byte(arr[i], 1);
    }
    return 0;
}

//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
