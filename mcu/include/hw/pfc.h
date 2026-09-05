/** ---------------------------------------------------------------------------
 * 
 * GORTOS
 * port/peripheral function control/configuration library
 * 
 * created by rdupu13
 * 
 * @file pfc.h
 * controls the port functions and peripherals of the specific mcu in use
 * 
 * mcu:     MSP430FR2153
 * package: TDBT 38-pin (34 GPIO + 2 prog + 2 power)
 *
----------------------------------------------------------------------------- */

//-----------------------------------------------------------------------------
//  HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef PFC_H
#define PFC_H


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

// hardware
#include <msp430fr2153.h>


//-----------------------------------------------------------------------------
//  MACROS
//-----------------------------------------------------------------------------

/*-------------------------------------
MSP430FR2153 PIN ASSIGNMENTS
LHS:
1)  P3.2 = 
2)  P3.1 = 
3)  P3.0 = 
4)  P1.3 = 
5)  P1.2 = LEDSTICK
6)  P1.1 = LED_TEST0
7)  P1.0 = LED_HEARTBEAT
8)  SBWTCK
9)  SBWTDIO
10) DVCC = 3.3v (max 4.1v, max 50 mA)
11) DVSS = 0v
12) P2.7 = LEDBAR_BIT7
13) P2.6 = LEDBAR_BIT6
14) P2.5 = LEDBAR_BIT5
15) P2.4 = LEDBAR_BIT4
16) P4.7 = UCB1SCL
17) P4.6 = UCB1SDA
18) P4.5 = LED_TEST1
19) P4.4 = LED_TEST2
---------------------------------------
RHS:
38) P3.3 = 
37) P5.0 = ADC_CH0
36) P5.1 = ADC_CH1
35) P3.4 = 
34) P3.5 = 
33) P3.6 = SWITCH1
32) P3.7 = SWITCH0
31) P1.4 = SPI_CS0
30) P1.5 = UCA0CLK
29) P1.6 = UCA0MISO
28) P1.7 = UCA0MOSI
27) P2.0 = LEDBAR_BIT0
26) P2.1 = LEDBAR_BIT1
25) P2.2 = LEDBAR_BIT2
24) P2.3 = LEDBAR_BIT3
23) P4.0 = LEDBAR_BIT8
22) P4.1 = LEDBAR_BIT9
21) P4.2 = UCA1RXD
20) P4.3 = UCA1TXD
---------------------------------------
*/

// adc ----------------------------------------------------
// channel 0 (A8)
#define ADC_CH0_SEL0    P5SEL0
#define ADC_CH0_SEL1    P5SEL1
#define ADC_CH0_PIN     BIT0
#define ADC_CH0_INCH    ADCINCH_8
// channel 1 (A9)
#define ADC_CH1_SEL0    P5SEL0
#define ADC_CH1_SEL1    P5SEL1
#define ADC_CH1_PIN     BIT1
#define ADC_CH1_INCH    ADCINCH_9
// TODO: peripheral base addr
// --------------------------------------------------------

// i2c ----------------------------------------------------
#define I2C_VECTOR      EUSCI_B1_VECTOR

#define I2C_SEL0        P4SEL0
#define I2C_PINS        BIT6 | BIT7
// TODO: peripheral base addr
// --------------------------------------------------------

// led ----------------------------------------------------
#define LED_HEARTBEAT_PORT  P1OUT
#define LED_HEARTBEAT_PIN   BIT0

#define LED_TEST0_PORT      P1OUT
#define LED_TEST0_PIN       BIT1
#define LED_TEST1_PORT      P4OUT
#define LED_TEST1_PIN       BIT5
#define LED_TEST2_PORT      P4OUT
#define LED_TEST2_PIN       BIT4
#define LED_TEST3_PORT      P1OUT
#define LED_TEST3_PIN       BIT2

#define LEDBAR_PORT0        P2OUT
#define LEDBAR_PORT1        P4OUT
#define LEDBAR_BIT8         BIT0
#define LEDBAR_BIT9         BIT1

#define LEDSTICK_PORT       LED_TEST3_PORT
#define LEDSTICK_PIN        LED_TEST3_PIN
// --------------------------------------------------------

// spi ----------------------------------------------------
// TODO: peripheral base addr
#define SPI_VECTOR      EUSCI_A0_VECTOR
#define SPI_SEL0        P1SEL0
#define SPI_PINS        BIT5 | BIT6 | BIT7 // 3-pin mode

#define SPI_CS0_PORT    P1OUT
#define SPI_CS0_PIN     BIT4

#define SPI_CS1_PORT // chip select 1 port
#define SPI_CS1_PIN // chip select 1 port

#define SPI_CS2_PORT // chip select 2 port
#define SPI_CS2_PIN // chip select 2 port

#define SPI_CS3_PORT // chip select 3 port
#define SPI_CS3_PIN // chip select 3 port
// --------------------------------------------------------

// switch -------------------------------------------------
// TODO: peripheral base addr
#define SWITCH_VECTOR   PORT3_VECTOR
#define SWITCH_IV       P3IV

// TODO: peripheral base addr
#define SWITCH0_DIR     P3DIR
#define SWITCH0_REN     P3REN
#define SWITCH0_OUT     P3OUT
#define SWITCH0_IN      P3IN
#define SWITCH0_IES     P3IES
#define SWITCH0_IE      P3IE
#define SWITCH0_IFG     P3IFG
#define SWITCH0_PIN     BIT7
#define SWITCH0_IV_VAL  0x10

// TODO: peripheral base addr
#define SWITCH1_DIR     P3DIR
#define SWITCH1_REN     P3REN
#define SWITCH1_OUT     P3OUT
#define SWITCH1_IN      P3IN
#define SWITCH1_IES     P3IES
#define SWITCH1_IE      P3IE
#define SWITCH1_IFG     P3IFG
#define SWITCH1_PIN     BIT6
#define SWITCH1_IV_VAL  0x0E
// --------------------------------------------------------

// timer --------------------------------------------------
// TODO: peripheral base addr
#define TIMER_B0_SRC    TBSSEL__ACLK    // source = aclk (32.768 kHz)
#define TIMER_B0_DIV    ID__1           // divide by 1
#define TIMER_B0_LEN    CNTL_0          // len = 16-bit
#define TIMER_B0_MODE   MC__UP          // up mode

#define TIMER_B1_SRC    TBSSEL__ACLK    // source = aclk (32.768 kHz)
#define TIMER_B1_DIV    ID__1           // divide by 1
#define TIMER_B1_LEN    CNTL_0          // len = 16-bit
#define TIMER_B1_MODE   MC__UP          // up mode

#define TIMER_B2_SRC    TBSSEL__ACLK    // source = aclk (32.768 kHz)
#define TIMER_B2_DIV    ID__1           // divide by 1
#define TIMER_B2_LEN    CNTL_0          // len = 16-bit
#define TIMER_B2_MODE   MC__CONTINOUS   // continuous mode
// --------------------------------------------------------

// uart ---------------------------------------------------
// TODO: peripheral base addr
#define UART_VECTOR     EUSCI_A1_VECTOR
#define UART_SEL0       P4SEL0
#define UART_PINS       BIT2 | BIT3
// --------------------------------------------------------

// board configuration ------------------------------------
// rtc (i2c)
#define RTC_MCP7940N

// mmm (spi)
#define MMM_23LC1024
#define MMM_SPI_SLAVE_NUM 0

// lcd (gpio)
#define LCD_NHD_0420AZ_FL_YBW_33V3

// --------------------------------------------------------

// lora ---------------------------------------------------
//#define LORA_RESET_PORT         P3OUT
//#define LORA_RESET_PIN          BIT5
// --------------------------------------------------------

//-----------------------------------------------------------------------------
//  FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

void pfc_init(void); // initialize ports


#endif
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
