/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * gort kernel
 * 
 * created by rdupu13
 * 
 * @file main.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

// kernel
#include "kernel/init.h"

// applications
#include "apps/gsh.h"

// testing
#include "kernel/gsys.h"
#include "drivers/rtc.h"
#include "drivers/adc.h"
#include "kernel/gio.h"
#include "kernel/gstr.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief gort kernel!
 * 
 * @param none
 * @return never returns!
 */
int main(void)
{
    init(); // initialize gort system

    //int gsh = main_gsh(); // launch gort shell

    char wave_str[5];
    wave_str[4] = '\0';
    while (1) {
        hex(wave_str, adc_read(0));
        helloworld(wave_str);
        helloworld("\n");

        rtc_get();
        
        eep(100);
    } // loop forever
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
