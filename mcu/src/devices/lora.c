/** ---------------------------------------------------------------------------
 *
 * GORTOS
 * rfm95w lora radio module driver
 * 
 * created by rdupu13
 * 
 * @file lora.c
 *
----------------------------------------------------------------------------- */


//-----------------------------------------------------------------------------
//  LIBRARIES
//-----------------------------------------------------------------------------

#include "devices/lora.h"

// drivers
#include "drivers/spi.h"

// kernel
#include "kernel/gio.h"
#include "kernel/gstr.h"


//-----------------------------------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------------------------------

volatile unsigned char reg_addrs[] = {
    LORA_REG_OP_MODE, LORA_REG_FRF_MSB, LORA_REG_FRF_MID, LORA_REG_FRF_LSB, LORA_REG_PA_CONFIG,
    LORA_REG_LNA, LORA_REG_FIFO_TX_BASE_ADDR, LORA_REG_FIFO_RX_BASE_ADDR,
    LORA_REG_MODEM_CONFIG_3, LORA_REG_SYNC_WORD, LORA_REG_VERSION, LORA_REG_PA_DAC
};
volatile unsigned char reg_addrs_len = sizeof(reg_addrs) / sizeof(reg_addrs[0]);;


//-----------------------------------------------------------------------------
//  FUNCTIONS
//-----------------------------------------------------------------------------

void lora_write_register(unsigned char reg_addr, unsigned char data);
unsigned char lora_read_register(unsigned char reg_addr);

/**
 * @brief initialize lora radio
 * 
 * @param freq_h frequency high int of radio (use preset macros LORA_FREQ_xxxMHZ_H)
 * @param freq_l frequency low int of radio (use preset macros LORA_FREQ_xxxMHZ_L)
 * 
 * @return 0 on success, -1 on error
 */
int lora_init(unsigned int freq_h, unsigned int freq_l)
{
    // Select GPIO used for reset pin
    switch(LORA_RESET_GPIO)
    {
        case 0: *lora_reset_gpio = &GPIO_00_OUT; break;
        default: *lora_reset_gpio = &GPIO_00_OUT; break;
    }

    // Reset SX1276 chip
    *lora_reset_gpio &= ~LORA_RESET_PIN;
    delay(320); // Delay for 10 ms (if this is even accurate...?)
    *lora_reset_gpio |= LORA_RESET_PIN;
    
    // Check version, do not continue if expected value not read (means radio isn't functioning)
    unsigned char version = lora_read_register(LORA_REG_VERSION);
    if (version != LORA_EXPECTED_VERSION) {
        return -1;
    }
    
    // Put into sleep mode for configuration
    // (Bit 7)    LoRa mode
    // (Bit 6)    Access LoRa registers page
    // (Bit 3)    High frequency mode (access to HF test registers)
    // (Bits 2:0) SLEEP mode
    lora_write_register(LORA_REG_OP_MODE, 0x80);

    // set frequency
    // frf = (frequency << 19) / 32,000,000
    lora_write_register(LORA_REG_FRF_MSB, (unsigned char) freq_h);
    lora_write_register(LORA_REG_FRF_MID, (unsigned char) ((freq_l >> 8) & 0x00FF));
    lora_write_register(LORA_REG_FRF_LSB, (unsigned char) freq_l);

    // Reset FIFO base addresses
    lora_write_register(LORA_REG_FIFO_TX_BASE_ADDR, 0x00);
    lora_write_register(LORA_REG_FIFO_RX_BASE_ADDR, 0x00);
    
    // LNA boost
    // (Bits 1:0) High frequency LNA current adjustment = Boost on, 150% LNA current
    unsigned char reg_lna = lora_read_register(LORA_REG_LNA);
    reg_lna |= 0x03;
    lora_write_register(LORA_REG_LNA, reg_lna);

    // Enable auto AGC
    // (Bit 2) AgcAutoOn = LNA gain set by internal AGC loop
    unsigned char modem_config_3 = 0x04;
    lora_write_register(LORA_REG_MODEM_CONFIG_3, modem_config_3);

    // Leave PA DAC alone?
    // TODO: Understand this
    // (Bit 7)    Reserved, retain default value of 1 for some reason?
    // (Bits 2:0) PaDac = Default value?
    unsigned char pa_dac = 0x84;
    lora_write_register(LORA_REG_PA_DAC, pa_dac);

    // Set TX power
    // TODO: Make configurable?
    // (Bit 7)    Set PA output pin to PA_BOOST, output power is limited to +20 dBm
    // (Bits 6:4) Max power = 10.8 [Pmax = 10.8 + 0.6*MaxPower]
    // (Bits 3:0) Output power = 17 [Pout = 17 - (15 - OutputPower)]
    unsigned char pa_config = 0x8F;
    lora_write_register(LORA_REG_PA_CONFIG, pa_config);    

    // Enter standby
    // (Bits 2:0) STDBY mode
    lora_write_register(LORA_REG_OP_MODE, 0x81);
    
    // Put all expected register values together
    unsigned char vals[] = {
        0x81, (unsigned char) freq_h, (unsigned char) ((freq_h >> 8) & 0x00FF), (unsigned char) freq_l, pa_config,
        reg_lna, 0x00, 0x00,
        modem_config_3, 0x12, LORA_EXPECTED_VERSION, pa_dac
    };

    // Verify that every register was written correctly using above array
    for (int i = 0; i < reg_addrs_len; i++)
    {
        unsigned char val = lora_read_register(reg_addrs[i]);
        
        // If any one register has the wrong value, return an error
        if (val != vals[i]) {
            // TODO: Make error more informative
            return -1;
        }
    }

    return 0;
}

/**
 * @brief write to a lora radio register over spi
 * 
 * @param reg_addr  address of register to write to
 * @param data      byte to be written
 * 
 * @return none
 */
void lora_write_register(unsigned char reg_addr, unsigned char data)
{
    *lora_spi_pckt_size = 2;
    *lora_spi_txbuf = reg_addr | 0x80;
    
    while (*lora_spi_stat & BIT_2);
    
    *lora_spi_txbuf = data;
    
    while (*lora_spi_stat & BIT_2);
}

/**
 * @brief read from a lora radio register over SPI
 * 
 * @param reg_addr address of register to read from
 * 
 * @return byte that was read
 */
unsigned char lora_read_register(unsigned char reg_addr)
{
    *lora_spi_pckt_size = 2;
    *lora_spi_txbuf = reg_addr & 0x7F;    
    
    while (*lora_spi_stat & BIT_2);
    
    *lora_spi_txbuf = 0x00;
    
    while (*lora_spi_stat & BIT_2);
    
    return *lora_spi_rxbuf;
}

/**
 * @brief Send a packet with the LoRa radio transceiver
 * 
 * @param data Payload to be sent
 * @param len Length of payload in bytes
 * 
 * @return 0 on success, -1 on TX busy timeout, -2 on TXDONE timeout
 */
int lora_send_packet(unsigned char *data, unsigned int len)
{
    int timeout = LORA_TIMEOUT_CNT;
    
    // Wait for radio to not be in TX mode
    // Timeout counter prevents an infinite loop
    unsigned char op_mode;
    do {
        op_mode = lora_read_register(LORA_REG_OP_MODE);
        timeout--;
    } while (((op_mode & 0x07) == 0x03) && (timeout > 0));
    
    // On timeout, clear IRQ flags and return to standby mode before returning an error
    if (timeout == 0) {
        lora_write_register(LORA_REG_IRQ_FLAGS, 0xFF);
        lora_write_register(LORA_REG_OP_MODE, 0x81);
        return -1;
    }
    timeout = LORA_TIMEOUT_CNT;

    // Put into standby mode
    lora_write_register(LORA_REG_OP_MODE, 0x81);

    // Use explicit header mode
    // (Clear bit 0 of RegModemConfig1)
    unsigned char modem_config_1 = lora_read_register(LORA_REG_MODEM_CONFIG_1);
    modem_config_1 &= 0xFE;
    lora_write_register(LORA_REG_MODEM_CONFIG_1, modem_config_1);

    // Reset FIFO SPI pointer
    lora_write_register(LORA_REG_FIFO_ADDR_PTR, 0x00);

    // Set payload length
    lora_write_register(LORA_REG_PAYLOAD_LENGTH, (unsigned char) len);

    // Write each payload byte to FIFO
    for (int i = 0; i < len; i++) {
        lora_write_register(LORA_REG_FIFO, data[i]);
    }

    // Clear IRQ flags before transmitting
    lora_write_register(LORA_REG_IRQ_FLAGS, 0xFF);

    // Put into TX mode
    // Radio automatically transmits what was put into FIFO!
    lora_write_register(LORA_REG_OP_MODE, 0x83);

    // Wait for TXDONE flag to be set
    // Timeout counter prevents an infinite loop
    unsigned char irq_flags;
    do {
        irq_flags = lora_read_register(LORA_REG_IRQ_FLAGS);
        timeout--;
    } while(((irq_flags & BIT_3) == 0x00) && (timeout > 0));
    
    // Clear IRQ flags and return to standby mode whether a timeout occured or not
    lora_write_register(LORA_REG_IRQ_FLAGS, 0xFF);
    lora_write_register(LORA_REG_OP_MODE, 0x81);
    
    if (timeout == 0) {
        return -2;
    }

    return 0;
}

/**
 * @brief Receive a packet with the LoRa radio transceiver
 * 
 * @param data Location to store the received packet
 * @param rssi Pointer to store RSSI (signal strength) value of received packet, leave NULL if unused
 * 
 * @return Number of bytes in received packet on success, -1 on RXDONE timeout, -2 on CRC error
 */
int lora_recv_packet(unsigned char *data, int *rssi)
{
    // Put into standby mode
    lora_write_register(LORA_REG_OP_MODE, 0x81);

    // Use explicit header mode
    // (Clear bit 0 of RegModemConfig1)
    unsigned char modem_config_1 = lora_read_register(LORA_REG_MODEM_CONFIG_1);
    modem_config_1 &= 0xFE;
    lora_write_register(LORA_REG_MODEM_CONFIG_1, modem_config_1);

    // Reset FIFO SPI pointer
    lora_write_register(LORA_REG_FIFO_ADDR_PTR, 0x00);

    // Put into RX continuous mode
    lora_write_register(LORA_REG_OP_MODE, 0x85);

    // Wait for RXDONE flag to be set (when packet received)
    // Timeout counter prevents an infinite loop
    int timeout = LORA_TIMEOUT_CNT;
    unsigned char irq_flags;
    do {
        irq_flags = lora_read_register(LORA_REG_IRQ_FLAGS);
        timeout--;
    } while (((irq_flags & BIT_6) == 0x00) && (timeout > 0));

    // On timeout, clear IRQ flags and return to standby mode before returning an error
    if (timeout == 0) {
        lora_write_register(LORA_REG_IRQ_FLAGS, 0xFF);
        lora_write_register(LORA_REG_OP_MODE, 0x81);
        return -1;
    }

    // If CRC error flag is set, clear IRQ flags and return to standby mode before returning an error
    if (irq_flags & BIT_5) {
        lora_write_register(LORA_REG_IRQ_FLAGS, 0xFF);
        lora_write_register(LORA_REG_OP_MODE, 0x81);
        return -2;
    }
    
    // Read length of received packet
    unsigned char n_bytes = lora_read_register(LORA_REG_RX_NB_BYTES);

    // Set FIFO pointer to current RX pointer
    unsigned char fifo_rx_current_addr = lora_read_register(LORA_REG_FIFO_RX_CURRENT_ADDR);
    lora_write_register(LORA_REG_FIFO_ADDR_PTR, fifo_rx_current_addr);

    // Read LORA_REG_FIFO n_bytes times, store in provided data buffer
    for (int i = 0; i < n_bytes; i++) {
        *(data + i) = lora_read_register(LORA_REG_FIFO);
    }

    // Read RSSI if caller wants it
    if (rssi != NULL) {
        *rssi = lora_read_register(LORA_REG_PKT_RSSI_VALUE) - 157; // Offset to get correct dBm value
    }

    // Clear IRQ flags and return to standby mode
    lora_write_register(LORA_REG_IRQ_FLAGS, 0xFF);
    lora_write_register(LORA_REG_OP_MODE, 0x81);

    // Return number of bytes in packet
    return n_bytes;
}

/**
 * @brief dump contents of lora radio registers to gout
 * 
 * @return none
 */
void lora_dump_registers(void)
{
    helloworld("\nRadio register dump:\n----------------\n");

    unsigned char i;
    unsigned char val;
    for (i = 0; i < reg_addrs_len; i++)
    {
        val = lora_read_register(reg_addrs[i]);
        
        helloworld("reg ");
        helloworld(hex(reg_addrs[i]));
        helloworld(" = ");
        helloworld(hex(val));
        helloworld("\n");
    }
    
    helloworld("----------------\n\n\n");
}
//-----------------------------------------------------------------------------
//  END OF CODE
//-----------------------------------------------------------------------------
