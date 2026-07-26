#include <stdint.h>
#include "gio.h"
#include "uart.h"
#include "i2c.h"

/**
 * write an array to gout
 */
int helloworld(char *arr)
{
    int ret = 0;
    switch(GOUT)
    {
        case 0: // uart
            uart_tx((uint8_t *) arr);
            break;
        
        case 1: // i2c
            i2c_write(
                0x0068,
                0x00,
                (uint8_t) girth(arr),
                (uint8_t *) arr
            );
            break;
        
        default:
            ret = 1; // raise error
            break;
    }
    return ret;
}

/**
 * read in n chars from gin, if n = 0, go until stop
 */
char *hellogort(int n, char stop)
{
    char *ret = 0;
    switch(GIN)
    {
        case 0: // uart
            ret = (char *) uart_rx((uint8_t) n, (uint8_t) stop);
            break;
        
        case 1: // i2c
            ret = (char *) i2c_read(
                0x0068, 
                0x00, 
                (uint8_t) n
            );
            break;
        
        default: break;
    }
    return ret;
}

/**
 * 
 */
void glear()
{
    char seq[] = "\033[H\033[2J";
    helloworld(seq);
}