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

unsigned char rtc_second;
unsigned char rtc_minute;
unsigned char rtc_hour;
unsigned char rtc_weekday;
unsigned char rtc_date;
unsigned char rtc_month;
unsigned char rtc_year;

char rtc_dt_str[RTC_STRLEN + 1];

volatile unsigned char *rtc_display;
unsigned char rtc_display_num;

unsigned char rtc_initialized;
int rtc_error;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief initialize rtc
 * 
 * @return status of initialization via i2c
 *          0: ok
 *          -1: i2c bus busy
 *          -2: i2c 0 length error
 *          -3: i2c timeout error
 *          -4: i2c nack error
 */
int rtc_init(void)
{
    rtc_display_num = 0;

    rtc_second = 0x18;
    rtc_minute = 0x24;
    rtc_hour = 0x12;
    rtc_weekday = 0x06;
    rtc_date = 0x23;
    rtc_month = 0x12;
    rtc_year = 0x05;

    gopy(rtc_dt_str, RTC_INIT_DT_STR, girth(RTC_INIT_DT_STR) + 1);

    rtc_display_sel(0);
    
    //rtc_set(); // TODO: python gui via uart to set system time

    rtc_initialized = 1;

    // stop clock, set (fake) init time, start clock, get time
    int stat;
    stat = rtc_stop();
    if (stat) {
        gsys_log("rtc: error stopping:");
        gsys_log(hex(gabs(stat)));
        rtc_initialized = 0;
        return stat;
    }
    stat = rtc_set();
    if (stat) {
        gsys_log("rtc: error stopping:");
        gsys_log(hex(gabs(stat)));
        rtc_initialized = 0;
        return stat;
    }
    stat = rtc_start();
    if (stat) {
        gsys_log("rtc: start error:");
        gsys_log(hex(gabs(stat)));
        rtc_initialized = 0;
        return stat;
    }
    stat = rtc_get();
    if (stat) {
        gsys_log("rtc: start error:");
        gsys_log(hex(gabs(stat)));
        rtc_initialized = 0;
        return stat;
    }
    stat = rtc_get();
    if (stat) {
        gsys_log("rtc: start error:");
        gsys_log(hex(gabs(stat)));
        rtc_initialized = 0;
        return stat;
    }

    return 0;
}

/**
 * @brief start rtc
 * 
 * @return status of read/write:
 *          0: ok
 *          -1: i2c bus busy
 *          -2: i2c 0 length error
 *          -3: i2c timeout error
 *          -4: i2c nack error
 *          -5: rtc not initialized error
 */
int rtc_start(void)
{
    if (!rtc_initialized) { return -5; }

    unsigned char ctl_reg;

    int stat = i2c_read(
        (volatile unsigned char *) &ctl_reg,
        1,
        RTC_SLAVE_ADDR,
        RTC_REG_CTL
    );
    if (stat) {
        rtc_error = stat;
        return stat;
    }
    
    ctl_reg &= ~BIT7; // enable oscillator
    
    stat = i2c_write(
        (volatile unsigned char *) &ctl_reg,
        1,
        RTC_SLAVE_ADDR,
        RTC_REG_CTL
    );
    rtc_error = stat;
    return stat;
}

/**
 * @brief stop rtc
 * 
 * @return status of read/write:
 *          0: ok
 *          -1: i2c bus busy
 *          -2: i2c 0 length error
 *          -3: i2c timeout error
 *          -4: i2c nack error
 *          -5: rtc not initialized error
 */
int rtc_stop(void)
{
    if (!rtc_initialized) {
        rtc_error = -5;
        return -5;
    }

    unsigned char ctl_reg;

    int stat = i2c_read(
        (volatile unsigned char *) &ctl_reg,
        1,
        RTC_SLAVE_ADDR,
        RTC_REG_CTL
    );
    if (stat) {
        rtc_error = stat;
        return stat;
    }

    ctl_reg |= BIT7; // disable oscillator
    
    stat = i2c_write(
        &ctl_reg,
        1,
        RTC_SLAVE_ADDR,
        RTC_REG_CTL
    );
    rtc_error = stat;
    return stat;
}

/**
 * @brief get current rtc date and time
 * 
 * @return status of read:
 *          0: ok
 *          -1: i2c bus busy
 *          -2: i2c 0 length error
 *          -3: i2c timeout error
 *          -4: i2c nack error
 *          -5: rtc not initialized error
 */
int rtc_get(void)
{
    if (!rtc_initialized) {
        rtc_error = -5;
        return -5;
    }
    
    unsigned char dt[7];

    int stat = i2c_read(
        (volatile unsigned char *) dt,
        7,
        RTC_SLAVE_ADDR,
        RTC_REG_SEC
    );
    if (stat) {
        rtc_error = stat;
        return stat;
    }

    rtc_second = dt[0];
    rtc_minute = dt[1];
    rtc_hour = dt[2];
    rtc_weekday = dt[3];
    rtc_date = dt[4];
    rtc_month = dt[5];
    rtc_year = dt[6];

    rtc_error = 0;
    return 0;
}

/**
 * @brief set current rtc date and time
 * 
 * @return status of write:
 *           0: ok
 *          -1: i2c bus busy
 *          -2: i2c 0 length error
 *          -3: i2c timeout error
 *          -4: i2c nack error
 *          -5: rtc not initialized error
 */
int rtc_set(void)
{
    if (!rtc_initialized) {
        rtc_error = -5;
        return -5;
    }

    unsigned char dt[7];
    
    dt[0] = rtc_second;
    dt[1] = rtc_minute;
    dt[2] = rtc_hour;
    dt[3] = rtc_weekday;
    dt[4] = rtc_date;
    dt[5] = rtc_month;
    dt[6] = rtc_year;

    int stat = i2c_write(
        (volatile unsigned char *) dt,
        7,
        RTC_SLAVE_ADDR,
        RTC_REG_SEC
    );
    rtc_error = stat;
    return stat;
}

/**
 * @brief get current rtc date and time and convert to string
 * 
 * @return pointer to string representation of current rtc date and time
 *          null pointer means rtc_get() returned an error (read with rtc_error)
 */
char *rtc_getstr(void)
{
    int stat = rtc_get();
    if (stat) {
        rtc_error = stat;
        return (char *) 0;
    }

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
 * @brief select which register to display
 * 
 * @param sel date/time register to select relative to seconds
 * 
 * @return none
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
 * @brief select next register to display
 * 
 * @return none
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
