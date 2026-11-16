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

        // parse arguments
        if (gstr_cmp(args[0], "exit"))
        {
            helloworld("exiting gsh...\n\n\n");
            return 0;
        }
        else if (gstr_cmp(args[0], "time"))
        {
            // time ---------------------------------------
            if (argc > 3) {
                if (argc > 4) { helloworld("too many args\n"); continue; }

                unsigned int hour = dec_to_uint(args[1]);
                unsigned int min = dec_to_uint(args[2]);
                unsigned int sec = dec_to_uint(args[3]);

                helloworld("new time: ");
                helloworld(uint_to_dec(hour));
                helloworld(":");
                helloworld(uint_to_dec(min));
                helloworld(":");
                helloworld(uint_to_dec(sec));
                helloworld("\n");

                rtc_settime(hour, min, sec);
            }
            else if ((argc > 1) && (argc < 4)) { helloworld("too few args\n"); }
            else { print_systime(); helloworld("\n"); }
            // --------------------------------------------
        }
        else if (gstr_cmp(args[0], "date"))
        {
            // date ---------------------------------------
            if (argc > 3) {
                if (argc > 4) { helloworld("too many args\n"); continue; }

                unsigned int month = dec_to_uint(args[1]);
                unsigned int date = dec_to_uint(args[2]);
                unsigned int year = dec_to_uint(args[3]);

                helloworld("new date: ");
                helloworld(uint_to_dec(month));
                helloworld("-");
                helloworld(uint_to_dec(date));
                helloworld("-");
                helloworld(uint_to_dec(year));
                helloworld("\n");
                
                rtc_setdate(month, date, year);
            }
            else if ((argc > 1) && (argc < 4)) { helloworld("too few args\n"); }
            else { print_systime(); helloworld("\n"); }
            // --------------------------------------------
        }
        else if (gstr_cmp(args[0], "temp"))
        {
            // temp ---------------------------------------
            helloworld("temperature = ");
            helloworld(uint_to_dec(window_size));
            helloworld("\n");
            helloworld("averaging window size = ");
            helloworld(uint_to_dec(window_size));
            helloworld("\n");
            // --------------------------------------------
        }
        else if (gstr_cmp(args[0], "twin"))
        {
            // twin ---------------------------------------
            if (argc > 1) {
                if (argc > 2) { helloworld("too many args\n"); continue; }

                unsigned int twin = dec_to_uint(args[1]);

                if (twin > MAX_WINDOW_SIZE) { twin = MAX_WINDOW_SIZE; }
                if (twin < MIN_WINDOW_SIZE) { twin = MIN_WINDOW_SIZE; }
                temp_window_set_size(twin);
                
                helloworld("new temperature averaging window size = ");
                helloworld(uint_to_dec(window_size));
                helloworld("\n");
            }
            else
            {
                helloworld("temperature window size = ");
                helloworld(uint_to_dec(window_size));
                helloworld("\n");
            }
            // --------------------------------------------
        }
        else if (gstr_cmp(args[0], "lcdc"))
        {
            // lcdc ---------------------------------------
            if (argc > 1) {
                if (argc > 2) { helloworld("too many args\n"); continue; }

                unsigned int lcdc = dec_to_uint(args[1]);

                if (lcdc > MAX_CONTRAST) { lcdc = MAX_CONTRAST; }
                if (lcdc < MIN_CONTRAST) { lcdc = MIN_CONTRAST; }
                lcd_contrast_set(lcdc);
                
                helloworld("new lcd contrast = ");
                helloworld(uint_to_dec(lcd_contrast));
                helloworld("\n");
            }
            else
            {
                helloworld("lcd contrast = ");
                helloworld(uint_to_dec(lcd_contrast));
                helloworld("\n");
            }
            // --------------------------------------------
        }
        

        // ------------------------------------------------
    }
    
    return 0;
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
