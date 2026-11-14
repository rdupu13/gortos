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

// devices
#include "devices/rtc.h"
#include "devices/temp.h"
#include "devices/lcd.h"


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
        
        // split command into arguments
        argc = 0;
        unsigned int i = 0;
        unsigned int cmd_len = gstr_len(gsh_cmd);
        while (i < cmd_len)
        {
            args[argc] = gsh_cmd + i;
            while ((gsh_cmd[i] != ' ')
                && (gsh_cmd[i] != '\r')) {
                i++; 
            }
            argc++;
            gsh_cmd[i++] = '\0';
        }
        
        //helloworld(uint_to_dec(argc));
        //helloworld("\n");
        //for (i = 0; i < argc; i++)
        //{
        //    helloworld(args[i]);
        //    helloworld("\n");
        //}
        //helloworld("\n");

        // parse arguments
        if (gstr_cmp(args[0], "exit"))
        {
            helloworld("exiting gsh...\n\n\n");
            return 0;
        }
        else if (gstr_cmp(args[0], "time"))
        {
            if (argc > 3) {
                if (argc > 4) { helloworld("too many args\n"); continue; }

                // TODO: dec_to_uint!!
                rtc_settime(11, 13, 0);

                helloworld("new time: ");
                print_systime();
                helloworld("\n");
            }
            else if ((argc > 1) && (argc < 4)) { helloworld("too few args\n"); }
            else { print_systime(); helloworld("\n"); }
        }
        else if (gstr_cmp(args[0], "date"))
        {
            if (argc > 3) {
                if (argc > 4) { helloworld("too many args\n"); continue; }

                // TODO: dec_to_uint!!
                rtc_setdate(11, 13, 26);

                helloworld("new date: ");
                print_systime();
                helloworld("\n");
            }
            else if ((argc > 1) && (argc < 4)) { helloworld("too few args\n"); }
            else { print_systime(); helloworld("\n"); }
        }
        else if (gstr_cmp(args[0], "twin"))
        {
            if (argc > 1) {
                if (argc > 2) { helloworld("too many args\n"); continue; }

                // TODO: dec_to_uint!!
                
                /*
                unsigned int win = dec_to_uint(args[1]);

                helloworld(uint_to_dec(win));
                helloworld("\n");

                if (win > MAX_WINDOW_SIZE) { win = MAX_WINDOW_SIZE; }
                if (win < MIN_WINDOW_SIZE) { win = MIN_WINDOW_SIZE; }
                temp_window_set_size(win);
                
                helloworld("temp window size = ");
                helloworld(uint_to_dec(window_size));
                helloworld("\n");
                */
            }
            else { helloworld(uint_to_dec(window_size)); helloworld("\n"); }
        }
        else if (gstr_cmp(args[0], "lcdc"))
        {
            if (argc > 1) {
                if (argc > 2) { helloworld("too many args\n"); continue; }

                // TODO: dec_to_uint!!
                
                /*
                unsigned int con = dec_to_uint(args[1]);

                helloworld(uint_to_dec(con));
                helloworld("\n");

                if (con > MAX_CONTRAST) { con = MAX_CONTRAST; }
                if (con < MIN_CONTRAST) { con = MIN_CONTRAST; }
                lcd_contrast_set(con);
                
                helloworld("lcd contrast = ");
                helloworld(uint_to_dec(con));
                helloworld("\n");
                */
            }
            else { helloworld(hex(lcd_contrast)); helloworld("\n"); }
        }
        

        // ------------------------------------------------
    }
    
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
