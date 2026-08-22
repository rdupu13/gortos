;------------------------------------------------------------------------------
; asm/main.s — MSP430 assembly for msp430-elf-gcc
;------------------------------------------------------------------------------
#include <msp430.h>

        .global main
        .text

;------------------------------------------------------------------------------
; MACROS
;------------------------------------------------------------------------------

        ; delay between clock toggles and such
        .macro i2c_delay
        nop
        nop
        nop
        .endm

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

;------------------------------------------------------------------------------
; SUBROUTINES
;------------------------------------------------------------------------------

; send start condition + slave address --------------------
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

        ; send slave address: ---------
        mov.w   #8, R4
byte_loop:
        ; left shift slave address, set/clear sda according to carry
        rlc.b   R5
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
        i2c_delay
        clear_scl
        i2c_delay

        dec.w   R4
        jnz     byte_loop
        ; -----------------------------

        clear_sda
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

        ; scl = P3.1
        bis.b   #BIT1, &P3DIR
        set_scl

        ; sda = P3.5
        bis.b   #BIT5, &P3DIR
        set_sda

loop:
        call    #i2c_tx_start   ; send start condition
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

        .data           ; static data at 0x2000

i2c_slave_addr:
        .byte 0x68      ; rtc slave address
i2c_mode:
        .byte 1         ; write mode

;------------------------------------------------------------------------------
; END OF CODE
;------------------------------------------------------------------------------
