/**
 * GORT SHELL APPLICATION
 * 
 * Ryan Dupuis
 */

#include "rtc.h"
#include "gio.h"
#include "gstr.h"

volatile char *gsh_cmd;
volatile char prompt[] = "gsh@/";

/**
 * The Gort Shell
 * 
 */
int main_gsh()
{
    int sel = 0;
    while(1)
    {
        helloworld(prompt);
        gsh_cmd = hellogort(0, '\r');
        helloworld("\n");
        // ----------------------------------------------------
        
        // print rtc time
        char *dt;
        dt = rtc_get_str();
        helloworld(dt);
        
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
