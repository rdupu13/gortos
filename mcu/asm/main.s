;------------------------------------------------------------------------------
; asm/main.s — MSP430 assembly for msp430-elf-gcc
;------------------------------------------------------------------------------
#include <msp430.h>

        .global main
        .text

;------------------------------------------------------------------------------
; MACROS
;------------------------------------------------------------------------------

        ; delay between things
        .macro i2c_delay
        nop
        nop
        .endm

        ; delay when scl is high
        .macro scl_high_delay
        nop
        nop
        nop
        nop
        nop
        nop
        .endm

        ; scl = P3.1
        .macro init_scl
        bis.b   #BIT1, &P3DIR
        set_scl
        .endm

        ; sda = P3.5
        .macro init_sda
        bis.b   #BIT5, &P3DIR
        set_sda
        .endm

        ; config sda as input
        .macro sda_input
        bic.b   #BIT5, &P3DIR
        .endm

        ; read sda during rx
        .macro sda_rx
        mov.b   &P3IN, &i2c_sda_rx
        and.b   #BIT5, &i2c_sda_rx
        .endm

; set/clear i2c bus ---------------------------------------
        ; scl = 0
        .macro clear_scl
        bic.b   #BIT1, &P3OUT
        .endm

        ; scl = 1
        .macro set_scl
        bis.b   #BIT1, &P3OUT
        .endm

        ; sda = 0
        .macro clear_sda
        bic.b   #BIT5, &P3OUT
        .endm

        ; sda = 1
        .macro set_sda
        bis.b   #BIT5, &P3OUT
        .endm
; ---------------------------------------------------------

;------------------------------------------------------------------------------
; SUBROUTINES
;------------------------------------------------------------------------------

; send byte and read ack ----------------------------------
; R4: counter
i2c_tx_byte:
        mov.w   #8, R4
byte_loop:
        ; left shift byte, set/clear sda according to carry
        rlc.b   &i2c_byte
        jnc     tx_0
tx_1:
        set_sda
        jmp     toggle_scl
tx_0:
        clear_sda

        ; clock data
toggle_scl:
        i2c_delay
        set_scl
        scl_high_delay
        clear_scl
        i2c_delay

        dec.w   R4
        jnz     byte_loop

        ; read sda for ack/nack
        sda_input
        set_sda

        i2c_delay
        set_scl
        scl_high_delay

        sda_rx

        clear_scl
        i2c_delay

        init_sda

        ; if nack received, send stop
        tst.b   &i2c_sda_rx
        jz      rx_ack
        call    #i2c_tx_stop
rx_ack:
        ret
; ---------------------------------------------------------

; send start condition + slave address --------------------
; R5: tmp
i2c_tx_start:
        ; send start condition
        clear_sda
        i2c_delay
        clear_scl
        i2c_delay

        ; move address over and set/clear r/w bit
        mov.b   &i2c_slave_addr, R5
        rla.b   R5
        bis.b   &i2c_mode, R5

        ; send slave address
        mov.w   R5, &i2c_byte
        call    #i2c_tx_byte

        ret
; ---------------------------------------------------------

; send stop condition -------------------------------------
i2c_tx_stop:
        clear_sda
        i2c_delay
        set_scl
        i2c_delay
        set_sda
        i2c_delay
        ret
; ---------------------------------------------------------


;------------------------------------------------------------------------------
; MAIN LOOP
;------------------------------------------------------------------------------

main:
        mov.w #WDTPW|WDTHOLD, &WDTCTL   ; stop watchdog timer
        bic.w #LOCKLPM5, &PM5CTL0       ; turn off low-power mode

        ; heartbeat led = P1.0
        bis.b   #BIT0, &P1DIR
        bic.b   #BIT0, &P1OUT

        init_scl
        init_sda

loop:
        call    #i2c_tx_start   ; send start condition + slave addr
        call    #i2c_tx_stop    ; send stop condition

        ; long delay ------------------
        mov.w #0xFFFF, R4
delay:
        nop
        nop
        nop
        dec.w R4
        jnz delay
        ; -----------------------------
            
        xor.b   #BIT0, &P1OUT   ; toggle heartbeat led

        jmp     loop
        ret

;------------------------------------------------------------------------------
; MEMORY ALLOCATION
;------------------------------------------------------------------------------

        .section .bss
i2c_byte:
        .skip 1
i2c_sda_rx:
        .skip 1

        .section .data
i2c_slave_addr:
        .byte 0x68      ; rtc slave address
i2c_mode:
        .byte 0         ; write mode

;------------------------------------------------------------------------------
; END OF CODE
;------------------------------------------------------------------------------
