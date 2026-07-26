#ifndef LED_H
#define LED_H

#include <msp430fr2153.h>
#include <stdint.h>

// NOTE: init.c sets all ports + pins to outputs by default

#define LED_HEARTBEAT_PORT  P1OUT
#define LED_HEARTBEAT_PIN   BIT0
#define LED_HEARTBEAT_QDIV  4       // only powers of 2

#define LED_TEST0_PORT      P1OUT
#define LED_TEST0_PIN       BIT1
#define LED_TEST1_PORT      P4OUT
#define LED_TEST1_PIN       BIT5
#define LED_TEST2_PORT      P4OUT
#define LED_TEST2_PIN       BIT4

#define LEDBAR_LEN          10
#define LEDBAR_PORT0        P2OUT
#define LEDBAR_PORT1        P4OUT
#define LEDBAR_BIT8         BIT0
#define LEDBAR_BIT9         BIT1

#define LEDBAR_PATTERN0_QDIV    4
#define LEDBAR_PATTERN1_QDIV    2
#define LEDBAR_PATTERN2_QDIV    2
#define LEDBAR_PATTERN3_QDIV    1
// pattern 4 is divided by 3 >:(
#define LEDBAR_PATTERN5_QDIV    4

#define LEDBAR_INOUT_LEN    8
#define LEDBAR_INOUT0       0x0030
#define LEDBAR_INOUT1       0x0048
#define LEDBAR_INOUT2       0x0084
#define LEDBAR_INOUT3       0x0102
#define LEDBAR_INOUT4       0x0201
#define LEDBAR_INOUT5       0x0102
#define LEDBAR_INOUT6       0x0084
#define LEDBAR_INOUT7       0x0048

#define LEDBAR_FILLR_LEN    10
#define LEDBAR_FILLR0       0x0200
#define LEDBAR_FILLR1       0x0300
#define LEDBAR_FILLR2       0x0380
#define LEDBAR_FILLR3       0x03C0
#define LEDBAR_FILLR4       0x03E0
#define LEDBAR_FILLR5       0x03F0
#define LEDBAR_FILLR6       0x03F8
#define LEDBAR_FILLR7       0x03FC
#define LEDBAR_FILLR8       0x03FE
#define LEDBAR_FILLR9       0x03FF

uint16_t ledbar_pattern0;
uint16_t ledbar_pattern1;
uint16_t ledbar_inout[LEDBAR_INOUT_LEN];
uint8_t ledbar_inout_cnt;
uint16_t ledbar_pattern2;
uint16_t ledbar_pattern3;
uint8_t ledbar_pattern4_qcnt;
uint16_t ledbar_pattern4;
uint16_t ledbar_fillr[LEDBAR_FILLR_LEN];
uint8_t ledbar_fillr_cnt;
uint16_t ledbar_pattern5;

uint16_t *ledbar_cur_pattern;

void led_init();
void led_heartbeat_update(int qcnt);
void ledbar_setpins();
void ledbar_update();
void ledbar_sel(int sel);

#endif
