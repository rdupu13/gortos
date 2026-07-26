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

#include <stdint.h>

#include "drivers/rtc.h"

#include "drivers/i2c.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------


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
    
    //rtc_set();

    rtc_start();
}

/**
 * @brief start rtc
 * 
 * @param none
 * 
 * @return none
 */
void rtc_start()
{
    uint8_t *ctl_reg;
    *ctl_reg = i2c_read(RTC_SLAVE_ADDR, RTC_REG_CTL, 1);
    *ctl_reg &= ~BIT7; // enable oscillator
    i2c_write(RTC_SLAVE_ADDR, RTC_REG_CTL, 1, ctl_reg);
}

/**
 * @brief stop rtc
 * 
 * @param none
 * 
 * @return none
 */
void rtc_stop()
{
    uint8_t *ctl_reg;
    *ctl_reg = i2c_read(RTC_SLAVE_ADDR, RTC_REG_CTL, 1);
    *ctl_reg |= BIT7; // disable oscillator
    i2c_write(RTC_SLAVE_ADDR, RTC_REG_CTL, 1, ctl_reg);
}

/**
 * @brief get current rtc date and time
 * 
 * @param none
 * 
 * @return none
 */
void rtc_get()
{
    uint8_t *dt = i2c_read(RTC_SLAVE_ADDR, RTC_REG_SEC, 7);

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
void rtc_set()
{
    rtc_stop();

    uint8_t dt[7];
    dt[0] = rtc_second;
    dt[1] = rtc_minute;
    dt[2] = rtc_hour;
    dt[3] = rtc_weekday;
    dt[4] = rtc_date;
    dt[5] = rtc_month;
    dt[6] = rtc_year;

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
char *rtc_getstr()
{
    rtc_get();
    
    rtc_dt_str[0] = ((rtc_month >> 4) & 0x0F) + '0';
    rtc_dt_str[1] = (rtc_month & 0x0F) + '0';
    rtc_dt_str[3] = ((rtc_date >> 4) & 0x0F) + '0';
    rtc_dt_str[4] = (rtc_date & 0x0F) + '0';
    rtc_dt_str[8] = ((rtc_year >> 4) & 0x0F) + '0';
    rtc_dt_str[9] = (rtc_year & 0x0F) + '0';
    rtc_dt_str[11] = ((rtc_hour >> 4) & 0x0F) + '0';
    rtc_dt_str[12] = (rtc_hour & 0x0F) + '0';
    rtc_dt_str[14] = ((rtc_minute >> 4) & 0x0F) + '0';
    rtc_dt_str[15] = (rtc_minute & 0x0F) + '0';
    rtc_dt_str[17] = ((rtc_second >> 4) & 0x0F) + '0';
    rtc_dt_str[18] = (rtc_second & 0x0F) + '0';
    
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

/**
 * 
 *
int bcd_to_int(uint8_t bcd)
{
    uint8_t ln = ln & 0x0F;
    uint8_t un = (bcd >> 4) & 0x0F;
    un = (un << 3) + (un << 1); // un = un * 10
    return (int) (un + ln);
}

/**
 * 
 *
void int_to_str(int n, char *str)
{
    int i = 0;

    if (n < 10) {
        str[i] = (char) (48 + n);
        return;
    }

    str[i] = '?';

    i++;
    str[i] = '\0';
}*/
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
