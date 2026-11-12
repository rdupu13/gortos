/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * gort system
 * 
 * created by rdupu13
 * 
 * @file gsys.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "kernel/gsys.h"

// hardware
#include "hw/pfc.h"

// drivers
#include "drivers/adc.h"
#include "drivers/i2c.h"
#include "drivers/led.h"
#include "drivers/patterns.h"
#include "drivers/spi.h"
#include "drivers/switch.h"
#include "drivers/timer.h"
#include "drivers/uart.h"

// devices
//#include "devices/dial.h"
#include "devices/ioexp.h"
#include "devices/lcd.h"
//#include "devices/mmm.h"
//#include "devices/rot.h"
#include "devices/rtc.h"
#include "devices/temp.h"

// kernel
//#include "kernel/gfs.h"
#include "kernel/gio.h"
#include "kernel/gstr.h"
//#include "kernel/menu.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char display_mode;

volatile gblk_t gblks[BLK_ALLOC_NUM];
volatile unsigned char gblks_data[BLK_ALLOC_NUM][BLK_SIZE];

unsigned int cur_qcnt;
unsigned int cur_fcnt;
int cur_temp;
unsigned int cur_speed;

char period_str[] = "0.0 s\n";


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

void die(char *last_words); // only gort can kill herself

unsigned char pot_get(unsigned char adc_ch);
void pot_set_speed(unsigned char adc_ch);

/**
 * @brief initialize gort system
 * 
 * @return none
 */
void gsys_init(void)
{
    pfc_init(); // initialize ports

    LED_HEARTBEAT_PORT |= LED_HEARTBEAT_PIN; // show signs of life

    // DRIVERS --------------------------------------------
    // initialize core gort peripheral drivers
    timer_init();
    adc_init();
    uart_init(96, 1); // 9600 baud, echo enabled
    i2c_init(60000, 1); // timeout = 60000, bit bang enabled
    spi_init(60000, 6); // timeout = 60000, divide clock by 6
    
    eep(INIT_EEP_PERIOD_MS); // eep for a lil to let clockies warm up
    
    // globally enable interrupts
    __asm__ __volatile__("nop");
    __asm__ __volatile__("eint" ::: "memory");
    __asm__ __volatile__("nop");

    // intialize digital i/o
    led_init();
    switch_init(0, 0);
    // ----------------------------------------------------

    // KERNEL -----------------------------------------------------------------
    display_mode = 1;
    gin = 0;
    gout = 0;
    glear();

    // DEVICES --------------------------------------------
    patterns_init();

    int rtc_stat = rtc_init(); // i2c
    if (rtc_stat) {
        die("gsys: initialization failed :(");
    } else {
        gsys_log("gsys: rtc init successful :)");
    }

    int ioexp_stat = ioexp_init(); // i2c
    if (ioexp_stat) {
        die("gsys: io expander init failed :(");
    } else {
        gsys_log("gsys: io expander init successful :)");
    }

    int lcd_stat = lcd_init(); // ioexp, spi
    if (lcd_stat) {
        die("gsys: lcd init failed :(");
    } else {
        gsys_log("gsys: lcd init successful :)");
    }
    
    //dial_init(); // led, switch

    //mmm_init(); // spi
    
    // ----------------------------------------------------    
    
    gout = 1;
    glear();
    gout = 0;
    
    //gfs_init(); // mmm
    
    // initialize gort blocks TODO: maybe clunky, but for fun
    int i;
    for (i = 0; i < BLK_ALLOC_NUM; i++) {
        int j;
        for (j = 0; j < BLK_SIZE; j++) {
            gblks_data[i][j] = 0;
        }
        gblks[i] = (gblk_t) {
            .num = i,
            .data = gblks_data[i]
        };
    }

    // print start message (init successful)
    helloworld("\n\n~~~ Gort OS ~~~\n");
    helloworld("(c) rdupu13 2026\n\n");
    helloworld("Current time: ");
    print_systime();
    helloworld("\n\n");
    // ------------------------------------------------------------------------
}

/**
 * @brief system update loop
 * 
 * @param div clock divider value, only powers of 2 (1, 2, 4, 8, ...)
 * 
 * @return none
 */
void gsys_update(void)
{
    while (cur_fcnt == timer_fcnt)
    {
        // do instantly:
    }

    // do every 0.125 s:
    helloworld(hex(timer_fcnt));
    helloworld("\n");

    // do every 0.25 s:
    if ((timer_fcnt & 1) == 0)
    {
        cur_temp = temp_analog_avg(0);
        
        pot_set_speed(1);
        
        rtc_get();

        update_lcd();
    }

    // do every 0.5 s:
    if ((timer_fcnt & 3) == 0)
    {
        
    }

    cur_qcnt = timer_qcnt;
    cur_fcnt = timer_fcnt;
}

/**
 * @brief add an entry to gort's diary
 * 
 * @param entry entry to be written
 * TODO: add name param
 * 
 * @return none
 */
void gsys_log(char *entry)
{
    //gout = " "; TODO: make dynamic

    helloworld("[ ");
    print_systime(); // TODO: maybe just time
    helloworld(" ] ");
    helloworld(entry);
    helloworld("\n");
}

/**
 * @brief gort died :(
 * 
 * @param last_words gort's last dying words
 * 
 * @return none
 */
void die(char *last_words)
{
    gsys_log(last_words);
    helloworld("\n\ngort died :(\nreboot now\n"); // final dying words
    
    // globally disable interrupts
    __asm__ __volatile__("dint" ::: "memory");
    __asm__ __volatile__("nop");

    led_init(); // nothing but ledbar test pattern
    //TODO: display error code?

    while (1) {} // dead for infinity
}

/**
 * @brief take a gort nap
 * 
 * @param delay duration of eep in ms
 * 
 * @return none
 */
void eep(unsigned int delay)
{
    volatile unsigned int i;
    volatile unsigned int j;
    for (i = 0; i < delay; i++) {
        for (j = 0; j < DELAY_LOOP_MS; j++) {} // ~1 ms delay loop
    }
}

// rtc ----------------------------------------------------
/**
 * @brief write current gort system time to gout
 * 
 * @return none
 */
void print_systime(void)
{
    char *systime;
    systime = rtc_getstr();
    if (!systime) {
        helloworld("(error getting rtc time)");
    } else {
        helloworld(systime);
    }
}
// --------------------------------------------------------

// led ----------------------------------------------------
/**
 * @brief blink a test led
 * 
 * @param led   test led number
 * @param delay duration of blink in ms
 * 
 * @return none
 */
void blinky(unsigned char led, unsigned int delay)
{
    led_on(led);
    eep(delay);
    led_off(led);
}

/**
 * @brief select current thing displayed on ledbar
 * 
 * @param sel selection
 * 
 * @return none
 */
void ledbar_sel(unsigned char sel)
{
    unsigned int n;
    switch(sel)
    {
        case 1: n = *cur_pattern; break;
        case 2: n = (unsigned int) *rtc_display; break;
        default: n = *cur_pattern; patterns_sel(-1); break;
    }

    ledbar_setpins(n);
}

/**
 * @brief 
 * 
 * @return 
 */
unsigned char pot_get(unsigned char adc_ch)
{
    unsigned int adc_val = adc_read(adc_ch);

    // convert adc_val into pattern period 0.10 - 1.00 step 0.05
    // T ~= 0.0234375*adc_val + 10
    return ((adc_val >> 6) + (adc_val >> 7) + 10);
}

/**
 * @brief 
 * 
 * @return 
 */
void pot_set_speed(unsigned char adc_ch)
{
    unsigned int period = (unsigned int) pot_get(adc_ch);

    // convert period 0.10 - 1.00 step 0.05 into capture-compare value
    // for 32.768 kHz clock, CNTL = 16:
    // CCR ~= 328*period
    unsigned int ccr_val = (period << 6) + (period << 4) + (period << 1);

    timer_ccr_set(0, 0, ccr_val);
}

/**
 * 
 */
void update_lcd(void)
{
    gout = 1;
    ghome();
            
    helloworld(cur_pattern_name);
    helloworld("      \n");

    helloworld("Period = ");
    char *raw_period_str = uint_to_dec(pot_get(1));
    if (gstr_len(raw_period_str) < 3) {
        period_str[0] = '0';
        period_str[2] = raw_period_str[0];
    } else {
        period_str[0] = raw_period_str[0];
        period_str[2] = raw_period_str[1];
    }
    helloworld(period_str);

    helloworld(uint_to_dec(cur_temp));
    helloworld(".5 ");
    helloworld("\xDF"); // degree sign
    helloworld("C       ");

    helloworld("N = ");
    helloworld(hex(window_size));
    helloworld("\n");

    print_systime();
    helloworld("\n");
    gout = 0;
}
// --------------------------------------------------------


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

/**
 * @brief update things (4 Hz) TODO: APP
 * 
 * @param qcnt timer quarter-second counter
 * 
 * @return none
 */
void qcnt_update(unsigned int qcnt)
{
    // update heartbeat led
    led_heartbeat_update(qcnt);
    
    // update patterns
    patterns_update(qcnt);

    // update ledbar
    ledbar_sel(display_mode);
}

/**
 * @brief update things faster (8 Hz)
 * 
 * @param fcnt timer "fast" counter
 * 
 * @return none
 */
void fcnt_update(unsigned int fcnt)
{
    
}

/**
 * @brief executes when switch 0 is pressed TODO: APP
 * 
 * @return none
 */
void switch_0_pressed(void)
{
    if (display_mode == 1)
    {
        display_mode = 2;
        led_on(0);
        rtc_display_sel(0);
    }
    else
    {
        display_mode = 1;
        led_off(0);
        patterns_sel(0);
    }
}

/**
 * @brief executes when switch 1 is pressed TODO: APP
 * 
 * @return none
 */
void switch_1_pressed(void)
{
    patterns_next();
    rtc_display_next();
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
