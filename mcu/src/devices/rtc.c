/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * rtc driver
 * 
 * created by rdupu13
 * 
 * @file rtc.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "devices/rtc.h"

// drivers
#include "drivers/i2c.h"

// kernel
#include "kernel/gstr.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

unsigned char rtc_display_num;

unsigned char rtc_second;
unsigned char rtc_minute;
unsigned char rtc_hour;
unsigned char rtc_weekday;
unsigned char rtc_date;
unsigned char rtc_month;
unsigned char rtc_year;

char rtc_dt_str[RTC_STRLEN];


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize rtc
 * 
 * @param none
 * @return none
 */
void rtc_init()
{
    rtc_display_num = 0;

    rtc_second = 0x00;
    rtc_minute = 0x23;
    rtc_hour = 0x12;
    rtc_weekday = 0x07;
    rtc_date = 0x25;
    rtc_month = 0x07;
    rtc_year = 0x26;

    rtc_dt_str[0]  = '1';
    rtc_dt_str[1]  = '0';
        rtc_dt_str[2]  = '-';
    rtc_dt_str[3]  = '1';
    rtc_dt_str[4]  = '3';
        rtc_dt_str[5]  = '-';
        rtc_dt_str[6]  = '2';
        rtc_dt_str[7]  = '0';
    rtc_dt_str[8]  = '0';
    rtc_dt_str[9]  = '3'; 
        rtc_dt_str[10] = ' ';
    rtc_dt_str[11] = '0';
    rtc_dt_str[12] = '3';
        rtc_dt_str[13] = ':';
    rtc_dt_str[14] = '4';
    rtc_dt_str[15] = '2';
        rtc_dt_str[16] = ':';
    rtc_dt_str[17] = '0';
    rtc_dt_str[18] = '0';
    
    //rtc_set(); // TODO: python gui to set system time
}

/**
 * @brief start rtc
 * 
 * @param none
 * 
 * @return none
 */
void rtc_start(void)
{
    unsigned char ctl_reg;

    // TODO: handle errors
    i2c_read(RTC_SLAVE_ADDR, RTC_REG_CTL, 1, &ctl_reg);
    ctl_reg &= ~BIT7; // enable oscillator
    i2c_write(RTC_SLAVE_ADDR, RTC_REG_CTL, 1, &ctl_reg);
}

/**
 * @brief stop rtc
 * 
 * @param none
 * 
 * @return none
 */
void rtc_stop(void)
{
    unsigned char ctl_reg;

    // TODO: handle errors
    i2c_read(RTC_SLAVE_ADDR, RTC_REG_CTL, 1, &ctl_reg);
    ctl_reg |= BIT7; // disable oscillator
    i2c_write(RTC_SLAVE_ADDR, RTC_REG_CTL, 1, &ctl_reg);
}

/**
 * @brief get current rtc date and time
 * 
 * @param none
 * 
 * @return none
 */
void rtc_get(void)
{
    unsigned char dt[7];

    // TODO: handle errors
    i2c_read(RTC_SLAVE_ADDR, RTC_REG_SEC, 7, dt);
    rtc_second = dt[0];
    rtc_minute = dt[1];
    rtc_hour = dt[2];
    rtc_weekday = dt[3];
    rtc_date = dt[4];
    rtc_month = dt[5];
    rtc_year = dt[6];
}

/**
 * @brief set current rtc date and time
 * 
 * @param none
 * 
 * @return none
 */
void rtc_set(void)
{
    rtc_stop();

    unsigned char dt[7];
    dt[0] = rtc_second;
    dt[1] = rtc_minute;
    dt[2] = rtc_hour;
    dt[3] = rtc_weekday;
    dt[4] = rtc_date;
    dt[5] = rtc_month;
    dt[6] = rtc_year;

    // TODO: handle errors
    i2c_write(RTC_SLAVE_ADDR, RTC_REG_SEC, 7, dt);

    rtc_start();
}

/**
 * @brief get current rtc date and time and convert to string
 * 
 * @param none
 * 
 * @return pointer to string representation of current rtc date and time
 */
char *rtc_getstr(void)
{
    rtc_get();
    
    hex_to_str(rtc_dt_str,      &rtc_month,  1);
    hex_to_str(rtc_dt_str + 3,  &rtc_date,   1);
    hex_to_str(rtc_dt_str + 8,  &rtc_year,   1);
    hex_to_str(rtc_dt_str + 11, &rtc_hour,   1);
    hex_to_str(rtc_dt_str + 14, &rtc_minute, 1);
    hex_to_str(rtc_dt_str + 17, &rtc_second, 1);

    return rtc_dt_str;
}

/**
 * 
 *
void rtc_setdate(int date, int month, int year)
{
    // TODO: bin to bcd

    rtc_set();
}

/**
 * 
 *
void rtc_settime(int date, int month, int year)
{
    // TODO: bin to bcd

    rtc_set();
}

// NON-I2C FUNCTIONS ----------------------------------------------------------

/**
 * 
 */
void rtc_display_sel(unsigned char sel)
{
    rtc_display_num = sel;
    switch(sel)
    {
        case 0: rtc_display = &rtc_second; break;
        case 1: rtc_display = &rtc_minute; break;
        case 2: rtc_display = &rtc_hour; break;
        case 3: rtc_display = &rtc_weekday; break;
        case 4: rtc_display = &rtc_date; break;
        case 5: rtc_display = &rtc_month; break;
        case 6: rtc_display = &rtc_year; break;
        default: break;
    }
}

/**
 * 
 */
void rtc_display_next(void)
{
    rtc_display_num++;
    if (rtc_display_num > 6)
    {
        rtc_display_num = 0;
    }
    rtc_display_sel(rtc_display_num);
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
