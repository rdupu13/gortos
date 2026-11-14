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

#define CMD_LEN     64
#define ARG_LEN     8
#define MAX_ARGS    8

char gsh_cmd[CMD_LEN];

unsigned char argc;
char *args[MAX_ARGS];

char cur_user[] = "gort";
char cur_path[] = "/";


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
        // clear buffer contents
        for (unsigned int i = 0; i < CMD_LEN; i++) {
            gsh_cmd[i] = '\0';
        }
        // read in next command
        hellogort(gsh_cmd);
        helloworld("\n");
        // ------------------------------------------------
        
        argc = 0;
        unsigned int i = 0;
        unsigned int cmd_len = gstr_len(gsh_cmd);
        //helloworld("cmd len: ");
        //helloworld(uint_to_dec(cmd_len));
        //helloworld("\n");

        while (i < cmd_len)
        {
            args[argc] = gsh_cmd + i;
            
            //helloworld(args[argc]);
            //helloworld("\n");

            while ((gsh_cmd[i] != ' ')
                && (gsh_cmd[i] != '\r'))
            {
                //helloworld(hex(i));
                //helloworld(":  ");
                //arf(gsh_cmd[i]);
                //helloworld("\n");
                i++; 
            }
            
            argc++;
            //helloworld("end of arg ");
            //helloworld(hex(argc));
            //helloworld(": ");
            //helloworld(hex(gsh_cmd[i]));
            //helloworld("\n");

            gsh_cmd[i++] = '\0';
        }
        
        helloworld("\n");
        helloworld(uint_to_dec(argc));
        helloworld(" args:\n");
        for (i = 0; i < argc; i++)
        {
            helloworld(args[i]);
            helloworld("\n");
        }

        // ------------------------------------------------
    }
    
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
