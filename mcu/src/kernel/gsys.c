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

unsigned int cur_fcnt;
int cur_temp;
unsigned int cur_speed;
unsigned char cur_contrast;

char period_str[] = "0.0 s";
char temp_str[] = "00.0\xDF";


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
    // must be banned from lcd_init for some reason:
    cur_contrast = 0;
    lcd_contrast_set(0);
    
    //dial_init(); // led, switch

    //mmm_init(); // spi
    
    // ----------------------------------------------------    
    
    gout = 1;
    glear();
    gout = 0;
    
    //gfs_init(); // mmm
    
    // initialize gort blocks
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
    helloworld("rdupu13 2026\n\n");
    helloworld("Current time: ");
    print_systime();
    helloworld("\n\n");
    // ------------------------------------------------------------------------
}

/**
 * @brief system update loop
 * 
 * @return none
 */
void gsys_update(void)
{
    while (cur_fcnt == timer_fcnt)
    {
        // do instantly:
    }
    cur_fcnt = timer_fcnt;

    // do every 0.125 s:
    //helloworld();
    //helloworld("\n");
    
    cur_temp = temp_analog_avg(0);
    pot_set_speed(1);
    
    // proof i'm not crazy:
    //helloworld(uint_to_dec(adc_read(0)));
    //helloworld("    ");
    //helloworld(uint_to_dec(cur_temp));
    //helloworld("\n");

    // do every 0.25 s:
    if ((timer_fcnt & 1) == 0)
    {
        rtc_get();
        update_lcd();
    }

    /*
    // do every 0.5 s:
    if ((timer_fcnt & 3) == 0)
    {
        
    }

    // do every 1 s:
    if ((timer_fcnt & 4) == 0)
    {
        
    }
    */
}

/**
 * @brief add an entry to gort's diary
 * 
 * @param entry entry to be written
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
    unsigned char period;

    // convert adc_val into pattern period 0.10 - 1.00 step 0.05
    // T ~= 0.0234375*adc_val + 10 (lookup table)
    if (adc_val < 228) { period = 10; }
    else if ((adc_val >= 228) && (adc_val < 455)) { period = 15; }
    else if ((adc_val >= 455) && (adc_val < 682)) { period = 20; }
    else if ((adc_val >= 682) && (adc_val < 910)) { period = 25; }
    else if ((adc_val >= 910) && (adc_val < 1137)) { period = 30; }
    else if ((adc_val >= 1137) && (adc_val < 1364)) { period = 35; }
    else if ((adc_val >= 1364) && (adc_val < 1591)) { period = 40; }
    else if ((adc_val >= 1591) && (adc_val < 1819)) { period = 45; }
    else if ((adc_val >= 1819) && (adc_val < 2046)) { period = 50; }
    else if ((adc_val >= 2046) && (adc_val < 2273)) { period = 55; }
    else if ((adc_val >= 2273) && (adc_val < 2501)) { period = 60; }
    else if ((adc_val >= 2501) && (adc_val < 2728)) { period = 65; }
    else if ((adc_val >= 2728) && (adc_val < 2955)) { period = 70; }
    else if ((adc_val >= 2955) && (adc_val < 3182)) { period = 75; }
    else if ((adc_val >= 3182) && (adc_val < 3410)) { period = 80; }
    else if ((adc_val >= 3410) && (adc_val < 3637)) { period = 85; }
    else if ((adc_val >= 3637) && (adc_val < 3864)) { period = 90; }
    else if ((adc_val >= 3864) && (adc_val < 4091)) { period = 95; }
    else if ((adc_val >= 4091) && (adc_val < 4096)) { period = 100; }
    else { period = 100; }

    return period;
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

    if (ccr_val > 32767) {
        ccr_val = 32767;
    }

    timer_ccr_set(0, 0, ccr_val);
}

/**
 * 
 */
void update_lcd(void)
{
    lcd_contrast_set(cur_contrast);

    gout = 1; // --------------------------------------------------------------
    ghome();
    
    // row 1: current pattern name ------------------------
    helloworld("\x01\x02\x03 ");
    helloworld(cur_pattern_name);
    //for (int i = 0; i < (15 - gstr_len(cur_pattern_name)); i++) {
    //    helloworld(" ");
    //}
    helloworld("\n");
    // ----------------------------------------------------

    // row 2: led patterns + heartbeat period -------------
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
    helloworld("      \n");
    // ----------------------------------------------------

    // row 3: temperature reading in celsius --------------
    char *raw_temp_str = uint_to_dec(cur_temp);
    if (gstr_len(raw_temp_str) < 3) {
        temp_str[0] = '0';
        temp_str[1] = raw_temp_str[0];
        temp_str[3] = raw_temp_str[1];
    } else {
        temp_str[0] = raw_temp_str[0];
        temp_str[1] = raw_temp_str[1];
        temp_str[3] = raw_temp_str[2];
    }
    helloworld(temp_str);
    helloworld("C       ");

    // temperature averaging window size
    helloworld("N = ");
    char *dec_window_size = uint_to_dec(window_size);
    if (gstr_len(dec_window_size) < 2) {
        helloworld("0"); // stupid required leading zero
    }
    helloworld(dec_window_size);
    helloworld("\n");
    // ----------------------------------------------------

    // row 4: print system time ---------------------------
    print_systime();
    // ----------------------------------------------------

    gout = 0; // --------------------------------------------------------------
}
// --------------------------------------------------------


//-----------------------------------------------------------------------------
//  INTERRUPT SERVICE ROUTINES
//-----------------------------------------------------------------------------

/**
 * @brief update things (variable)
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
 * @brief executes when switch 0 is pressed
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
 * @brief executes when switch 1 is pressed
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
