#ifndef RTC_H
#define RTC_H

#include <stdint.h>

#define RTC_SLAVE_ADDR 0x0068

#define RTC_REG_SEC 0x00
#define RTC_REG_CTL 0x0E

#define RTC_STRLEN 19

volatile uint8_t rtc_second;
volatile uint8_t rtc_minute;
volatile uint8_t rtc_hour;
volatile uint8_t rtc_weekday;
volatile uint8_t rtc_date;
volatile uint8_t rtc_month;
volatile uint8_t rtc_year;

volatile char rtc_dt_str[RTC_STRLEN];

void rtc_init(); // initialize rtc
void rtc_start(); // start rtc
void rtc_stop(); // stop rtc
void rtc_get(); // get current date + time
char *rtc_get_str();
void rtc_set(); // set current date + time
void rtc_set_date(int month, int day, int year);
void rtc_set_time(int hour, int minute, int second);

#endif