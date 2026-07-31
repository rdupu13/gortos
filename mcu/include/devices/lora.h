/** ---------------------------------------------------------------------------
 *
 * GORTOS
 * rfm95w lora radio module driver library
 * 
 * created by rdupu13
 * 
 * @file lora.h
 *
----------------------------------------------------------------------------- */


//-----------------------------------------------------------------------------
//	HEADER DEFINITION
//-----------------------------------------------------------------------------

#ifndef LORA_H
#define LORA_H


//-----------------------------------------------------------------------------
//	LIBRARIES
//-----------------------------------------------------------------------------

#include <msp430fr2153.h>


//-----------------------------------------------------------------------------
//	MACROS
//-----------------------------------------------------------------------------

// radio registers
#define LORA_REG_FIFO                 0x00
#define LORA_REG_OP_MODE              0x01

#define LORA_REG_FRF_MSB              0x06
#define LORA_REG_FRF_MID              0x07
#define LORA_REG_FRF_LSB              0x08
#define LORA_REG_PA_CONFIG            0x09

#define LORA_REG_OCP                  0x0B
#define LORA_REG_LNA                  0x0C
#define LORA_REG_FIFO_ADDR_PTR        0x0D
#define LORA_REG_FIFO_TX_BASE_ADDR    0x0E
#define LORA_REG_FIFO_RX_BASE_ADDR    0x0F
#define LORA_REG_FIFO_RX_CURRENT_ADDR 0x10

#define LORA_REG_IRQ_FLAGS            0x12
#define LORA_REG_RX_NB_BYTES          0x13

#define LORA_REG_PKT_SNR_VALUE        0x19
#define LORA_REG_PKT_RSSI_VALUE       0x1A
#define LORA_REG_RSSI_VALUE           0x1B

#define LORA_REG_MODEM_CONFIG_1       0x1D
#define LORA_REG_MODEM_CONFIG_2       0x1E

#define LORA_REG_PREAMBLE_MSB         0x20
#define LORA_REG_PREAMBLE_LSB         0x21
#define LORA_REG_PAYLOAD_LENGTH       0x22

#define LORA_REG_MODEM_CONFIG_3       0x26

#define LORA_REG_FREQ_ERROR_MSB       0x28
#define LORA_REG_FREQ_ERROR_MID       0x29
#define LORA_REG_FREQ_ERROR_LSB       0x2A

#define LORA_REG_RSSI_WIDEBAND        0x2C

#define LORA_REG_SYNC_WORD            0x39

#define LORA_REG_VERSION              0x42

#define LORA_REG_PA_DAC               0x4D

// pinout
#define LORA_RESET_PORT         P3OUT
#define LORA_RESET_PIN          BIT5

// frequencies
#define LORA_FREQ_905MHz_H      0x00E1
#define LORA_FREQ_905MHZ_L      0xC000

#define LORA_FREQ_915MHZ_H      0x00E4 // 915 MHz is most commonly used, only one tested so far
#define LORA_FREQ_915MHZ_L      0xC000

#define LORA_FREQ_923MHZ_H      0x00E6
#define LORA_FREQ_923MHZ_L      0xCCCD

// misc
#define LORA_EXPECTED_VERSION   0x12    // semtech id relating to silicon revision, should never change if using rfm95w lora module
#define LORA_TIMEOUT_CNT        60000   // number of times to poll radio status/flags before returning an error, tune to max packet size


//-----------------------------------------------------------------------------
//	FUNCTION PROTOTYPES
//-----------------------------------------------------------------------------

int lora_init(unsigned int freq_h, unsigned int freq_l);

int lora_send_packet(unsigned char *data, unsigned int len);
int lora_recv_packet(unsigned char *data, int *rssi);

void lora_dump_registers(void);


#endif
//-----------------------------------------------------------------------------
//	END OF CODE
//-----------------------------------------------------------------------------
