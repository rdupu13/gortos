/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * rtc driver library
 * 
 * created by rdupu13
 * 
 * @file rtc.h
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef RTC_H
#define RTC_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include <stdint.h>


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

#define RTC_SLAVE_ADDR 0x0068

#define RTC_REG_SEC 0x00
#define RTC_REG_CTL 0x0E

#define RTC_STRLEN 19


//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

volatile uint8_t rtc_second;
volatile uint8_t rtc_minute;
volatile uint8_t rtc_hour;
volatile uint8_t rtc_weekday;
volatile uint8_t rtc_date;
volatile uint8_t rtc_month;
volatile uint8_t rtc_year;

volatile char rtc_dt_str[RTC_STRLEN];

void rtc_init();                                    // initialize rtc
void rtc_start();                                   // start rtc
void rtc_stop();                                    // stop rtc
void rtc_get();                                     // get current rtc date and time
void rtc_set();                                     // set current date and time
char *rtc_getstr();                                 // get current rtc date and time and convert to string
//void rtc_setdate(int month, int day, int year);
//void rtc_settime(int hour, int minute, int second);


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
