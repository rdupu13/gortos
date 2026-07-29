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


// kernel
#include "kernel/gio.h"
#include "kernel/gstr.h"
#include "kernel/gsys.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile char gsh_cmd[64];

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

        hellogort(gsh_cmd, 0, '\r');
        // ----------------------------------------------------
        
        // print rtc time
        print_systime();

        // ----------------------------------------------------
        helloworld("\n");
    }
    
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
