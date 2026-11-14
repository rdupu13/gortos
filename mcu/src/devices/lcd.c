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

// drivers
#include "drivers/spi.h"

// devices
#include "devices/ioexp.h"

// kernel
#include "kernel/gsys.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

unsigned char lcd_reg;
unsigned char lcd_line;
unsigned char lcd_col;
unsigned char lcd_line_addr[NUM_LINES] = {0x00, 0x40, 0x14, 0x54};

unsigned char lcd_contrast;

unsigned char smile_char[] =
{
    0b00001110,
    0b00011111,
    0b00010101,
    0b00010101,
    0b00011111,
    0b00010101,
    0b00011011,
    0b00001110
};

unsigned char gort_char[] =
{
    0b00001110,
    0b00011111,
    0b00011111,
    0b00010101,
    0b00000100,
    0b00001110,
    0b00000100,
    0b00001010
};

unsigned char star_char[] =
{
    0b00000100,
    0b00000100,
    0b00001110,
    0b00011111,
    0b00001110,
    0b00001110,
    0b00011011,
    0b00010001
};

unsigned char heart_char[] =
{
    0b00001010,
    0b00011011,
    0b00011111,
    0b00011111,
    0b00001110,
    0b00001110,
    0b00000100,
    0b00000100
};

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
    // magic bit banging:
    eep(50);
    lcd_write_nibble(0x03, 0); eep(5);
    lcd_write_nibble(0x03, 0); eep(1);
    lcd_write_nibble(0x03, 0); eep(1);
    lcd_write_nibble(0x02, 0); eep(1);
    lcd_write_byte(0x28, 0); // 4-bit, 2 lines, 5x8 font
    lcd_write_byte(0x06, 0); // incerement cursor, no shift
    lcd_write_byte(0x0F, 0); // display on, cursor + blink on
    lcd_write_byte(0x01, 0); // clear display
    eep(5);

    lcd_cursor_show(0);

    lcd_char_set(1, smile_char);
    lcd_char_set(2, gort_char); // insha'allah gort must not have legs
    lcd_char_set(3, star_char);
    //lcd_char_set(4, heart_char);  // insha'allah gort says limit is 3 custom chars

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
    unsigned char base = ((nibble & 0x0F) << 4) | (reg_sel ? LCD_RS : 0);
    
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
        if (byte == '\n' || (lcd_col == NUM_COLS)) {
            lcd_line = (lcd_line + 1) & 0x03;
            lcd_cursor_goto(lcd_line, 0);
            if (byte == '\n') { return 0; }
        } 
    }
    lcd_write_nibble(byte >> 4, reg_sel);
    lcd_write_nibble(byte & 0x0F, reg_sel);
    if (reg_sel) {
        lcd_col++;
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
int lcd_write(unsigned char *arr)
{
    unsigned int i;
    for (i = 0; arr[i] != '\0'; i++)
    {
        lcd_write_byte(arr[i], 1);
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param 
 * 
 * @return 
 */
int lcd_cursor_show(unsigned char state)
{
    int ret;
    switch(state)
    {
        case 1: ret = lcd_write_byte(0x0F, 0); break;
        case 0: ret = lcd_write_byte(0x0C, 0); break;
        default: ret = -1; break;
    }
    return ret;
}

/**
 * @brief 
 * 
 * @param 
 * 
 * @return 
 */
int lcd_cursor_blink(unsigned char state)
{
    int ret;
    switch(state)
    {
        case 1: ret = lcd_write_byte(0x0F, 0); break;
        case 0: ret = lcd_write_byte(0x0E, 0); break;
        default: ret = -1; break;
    }
    return ret;
}

/**
 * @brief 
 * 
 * @param 
 * 
 * @return 
 */
int lcd_cursor_goto(
    unsigned char row,
    unsigned char col
) {
    lcd_line = row;
    lcd_col = col;
    return lcd_write_byte(0x80 | (lcd_line_addr[row] + col), 0);
}

/**
 * @brief
 * 
 * @param 
 * 
 * @return 
 */
int lcd_clear(void)
{
    return lcd_write_byte(0x01, 0);
}

/**
 * @brief
 * 
 * @param
 * 
 * @return 
 */
int lcd_test(void)
{
    int ret;
    char c; 
    for (c = 32; c < 128; c++) {
        ret = lcd_write_byte(c, 1);
        if (ret) { return ret; }
        eep(150);
    }
    eep(1000);
    return ret;
}

/**
 * @brief 
 * 
 * @param val contrast level 0-7
 * 
 * @return
 */
int lcd_contrast_set(unsigned char val)
{
    unsigned char dac_in[] = {0b00110000, 0b00000000};
    
    if (val > 7) {
        val = 7;
    }
    lcd_contrast = val;

    val = ((val & 0x0007) << 4);

    dac_in[0] |= ((val >> 4) & 0x0F);
    dac_in[1] |= ((val << 4) & 0xF0);
    
    int stat = spi_write(
        dac_in,
        2,
        DAC_SPI_CS,
        (unsigned char *) 0,
        0
    );
    return stat;
}

/**
 * @brief 
 * 
 * @param addr 
 * @param char_data 
 * 
 * @return 
 */
int lcd_char_set(
    unsigned char addr,
    unsigned char *char_data
) {
    if (addr > 7) {
        addr = 7;
    }

    // set address counter to cgram address
    lcd_write_byte(0x40 | (addr << 3), 0);

    // write custom character
    unsigned int i;
    for (i = 0; i < 8; i++) {
        lcd_write_byte(char_data[i], 1);
    }

    // return address counter to ddram
    lcd_write_byte(0x80, 0);

    return 0; // TODO: handle errors
}

//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
