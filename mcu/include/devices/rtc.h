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

volatile unsigned char *rtc_display; // register currently being displayed

void rtc_init(void);    // initialize rtc
void rtc_start(void);   // start rtc
void rtc_stop(void);    // stop rtc
void rtc_get(void);     // get current rtc date and time
void rtc_set(void);     // set current date and time

char *rtc_getstr(void); // get current rtc date and time and convert to string
//void rtc_setdate(int month, int day, int year);
//void rtc_settime(int hour, int minute, int second);

void rtc_display_sel(unsigned char sel);
void rtc_display_next(void);

#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
