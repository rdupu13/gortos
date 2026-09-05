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
#include "drivers/spi.h"
#include "drivers/switch.h"
#include "drivers/timer.h"
#include "drivers/uart.h"

// devices
//#include "devices/dial.h"
//#include "devices/lcd.h"
//#include "devices/lora.h"
//#include "devices/mmm.h"
#include "devices/patterns.h"
//#include "devices/pwm.h"
#include "devices/rtc.h"

// kernel
//#include "kernel/gfs.h"
#include "kernel/gio.h"
#include "kernel/gstr.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char display_mode;

volatile gblk_t gblks[BLK_ALLOC_NUM];
volatile unsigned char gblks_data[BLK_ALLOC_NUM][BLK_SIZE];

unsigned int cur_qcnt;
int cur_temp;
unsigned int cur_speed;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

void die(char *last_words);

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
    i2c_init(60000); // timeout = 60000
    spi_init(60000, 6); // timeout = 60000
    
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

    int rtc_stat = rtc_init();
    if (rtc_stat) {
        die("rtc: initialization error :(");
    } else {
        gsys_log("rtc: initialization successful :)");
    }

    //pwm_init(); // timer
    //dial_init(); // led, switch
    
    //lcd_init(); // i2c
    //mmm_init(); // spi
    //lora_init(); // spi
    // ----------------------------------------------------    

    //gfs_init(); // mmm, (uart, lcd, lora)?
    
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
 * 
 */
void gsys_update(unsigned int div)
{
    while ((cur_qcnt & ~(div - 1))
        == (timer_qcnt & ~(div - 1)))
    {
        // do instantly:
        cur_speed = (((adc_read(1) & 0xF00) >> 8)*1843) + 3276;
        timer_ccr_set(0, 0, cur_speed);
        
        cur_temp = temp_analog_avg(adc_read(0));
    }
    cur_qcnt = timer_qcnt;

    // do every 0.25 seconds:
    rtc_get();

    if ((timer_qcnt & 3) == 0)
    {
        // do every 1 second:
        helloworld("current time: ");
        print_systime();
        helloworld("\n");

        helloworld("current temperature: ");
        helloworld(hex(cur_temp));
        helloworld("h degrees C");
        
        helloworld("\npatterns speed: ");
        helloworld(hex(cur_speed));
        helloworld("h\n\n");

        unsigned char b = 0x4D;
        unsigned char s;
        unsigned int i;
        for (i = 0; i < 8; i++)
        {
            s = (b & 0x80);
            LEDSTICK_PORT &= ~LEDSTICK_PIN;
            if (s) {
                __asm__ __volatile__("nop");
                LEDSTICK_PORT |= LEDSTICK_PIN;
                b = b << 1;
            }
            else {
                LEDSTICK_PORT |= LEDSTICK_PIN;
                b = b << 1;
            }
        }
    }
    else
    {
        
    }
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
    //gout = " ";

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
 * @brief select current thing displayed on ledbar TODO: APP
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
 * @brief update things faster (256 Hz) TODO: APP
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
