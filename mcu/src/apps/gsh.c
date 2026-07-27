/** ---------------------------------------------------------------------------
 * 
 * GORTOS APPLICATION
 * The Gort Shell
 * 
 * created by rdupu13
 * 
 * @file gsh.c
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "apps/gsh.h"

// drivers
// TODO: create kernel interface, apps should not have direct access to drivers
#include "drivers/led.h"
#include "drivers/rtc.h"

// kernel
#include "kernel/gio.h"
#include "kernel/gstr.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile char *gsh_cmd;

volatile char cur_user[] = "gort";
volatile char cur_path[] = "/home/gort";


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * @brief The Gort Shell
 * 
 * @param none
 * 
 * @return process status
 */
int main_gsh()
{
    int sel = 0;
    while(1)
    {
        helloworld(cur_user);
        helloworld("@");
        helloworld(cur_path);
        helloworld("$ ");

        gsh_cmd = hellogort(0, '\r');
        // ----------------------------------------------------
        
        // print rtc time
        char *dt;
        dt = rtc_getstr();
        helloworld(dt);
        helloworld("\n");
        
        // switch ledbar selection
        sel++;
        if (sel > 5) {
            sel = 0;
        }
        ledbar_sel(sel);
        
        // ----------------------------------------------------
        helloworld("\n");
    }
    
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
