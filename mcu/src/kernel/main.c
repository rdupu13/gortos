#include <msp430fr2153.h>
#include "led.h"
#include "timer.h"
#include "uart.h"
#include "i2c.h"
#include "rtc.h"
#include "gio.h"
#include "gstr.h"
#include "init.h"

/**
 * gort kernel routine!
 */
int main(void)
{
    init(); // initialize gort system

    int gsh = main_gsh();

    int i = 0;
    while (1)
    {
        /*
        char *date;
        date = rtc_get_str();
        helloworld(date);
        helloworld("\n");
        */
        
        /*
        i++;
        if (i > 5) {
            i = 0;
        }
        ledbar_sel(i);
        */

        //helloworld("print\n");

        eep(696);

    } // loop forever
    return 0;
}
