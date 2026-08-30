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

// kernel
#include "kernel/gio.h"
#include "kernel/gstr.h"
#include "kernel/gsys.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

#define MAX_ARGS    10
#define CMD_LEN     64

char gsh_cmd[CMD_LEN];

char cur_user[] = "gort";
char cur_path[] = "/home/gort";


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

/**
 * 
 */
unsigned int strstr_len(char **strstr)
{
    unsigned int i;
    for (i = 0; strstr[i] != (char *) 0; i++) {}
    return i;
}

/**
 * @brief The Gort Shell
 * 
 * @param none
 * 
 * @return process status
 */
int main_gsh(void)
{
    while(1)
    {
        // print prompt
        helloworld(cur_user);
        helloworld("@");
        helloworld(cur_path);
        helloworld("$ ");

        // ------------------------------------------------
        // wait and read in next command
        hellogort(gsh_cmd, 0, '\r');
        // ------------------------------------------------
        
        // split command by spaces
        char *args[MAX_ARGS];
        gstr_split(args, gsh_cmd, ' ');

        // time command
        if (gstr_cmp(args[0], "time"))
        {
            // print rtc time
            helloworld("\n\n");
            print_systime();
            helloworld("\n");
        
            if (gstr_cmp(args[1], "set"))
            {
                helloworld("arguments given: ");
                helloworld(hex(strstr_len(args)));
                helloworld("\n");
            }
        }

        // exit command
        else if (gstr_cmp(args[0], "exit"))
        {
            helloworld("\nexiting gsh...\n");
            helloworld("\n");
            return 0;
        }

        helloworld("\n");
        // ------------------------------------------------
    }
    
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
