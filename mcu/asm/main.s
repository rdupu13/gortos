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

; send start condition + slave address --------------------
; R5: tmp
i2c_tx_start:
        ; reset nack for retry
        mov.b   #0, &i2c_nack

        ; make sure scl and sda start high
        init_scl
        init_sda
        i2c_delay

        ; send start condition
        clear_sda
        i2c_delay
        clear_scl
        i2c_delay

        ; move address over and set/clear r/w bit
        mov.b   &i2c_slave_addr, R5
        rla.b   R5
        bis.b   &i2c_mode, R5

        ; send slave address + r/w bit
        mov.b   R5, &i2c_byte
        call    #i2c_tx_byte

        ret
; ---------------------------------------------------------

; send stop condition -------------------------------------
i2c_tx_stop:
        ; if nack previously received, don't do anything
        tst.b   &i2c_nack
        jnz     end_tx_stop

        clear_sda
        i2c_delay
        set_scl
        i2c_delay
        set_sda
        i2c_delay

end_tx_stop:
        ret
; ---------------------------------------------------------

; tx byte and read ack ------------------------------------
; R4: counter
i2c_tx_byte:
        ; if nack previously received, don't do anything
        tst.b   &i2c_nack
        jnz     end_tx_byte

        ; clock 8 bits of data ----------------------------
        mov.b   #8, R4
tx_byte_loop:
        ; left shift byte, set/clear sda according to carry
        rlc.b   &i2c_byte
        jnc     tx_0
tx_1:
        set_sda
        jmp     toggle_scl
tx_0:
        clear_sda

toggle_scl:
        i2c_delay
        set_scl
        scl_high_delay
        clear_scl
        i2c_delay

        dec.b   R4
        jnz     tx_byte_loop
        ; -------------------------------------------------

        ; read sda for ack/nack
        sda_input
        set_sda

        i2c_delay
        set_scl
        
        i2c_delay
        sda_rx
        i2c_delay

        clear_scl
        i2c_delay

        init_sda

        ; if nack received, send stop
        tst.b   &i2c_sda_rx
        jz      end_tx_byte
        call    #i2c_tx_stop
        mov.b   #1, &i2c_nack
end_tx_byte:
        ret
; ---------------------------------------------------------

; rx byte and send ack ------------------------------------
; R4: counter
i2c_rx_byte:
        ; if nack previously received, don't do anything
        tst.b   &i2c_nack
        jnz     end_tx_byte

        sda_input

        ; clock 8 bits of data ----------------------------
        mov.b   #8, R4
rx_byte_loop:
        i2c_delay
        set_scl

        i2c_delay
        sda_rx
        i2c_delay

        clear_scl
        i2c_delay

        ; 
        tst.b   &i2c_sda_rx
        jz      rx_0
rx_1:
        bis.w   #BIT0, SR       ; set carry flag
        jmp     rs_byte
rx_0:
        bic.w   #BIT0, SR       ; clear carry flag
rs_byte:
        rlc.b   &i2c_byte

        dec.b   R4
        jnz     rx_byte_loop
        ; -------------------------------------------------

        init_sda

        ; send ack or nack (depending on if last byte)
        tst.b   &i2c_end_rx
        jz      tx_ack
tx_nack:
        set_sda
        mov.b   #0, &i2c_end_rx
        jmp     clk_ack
tx_ack:
        clear_sda

clk_ack:
        i2c_delay
        set_scl
        scl_high_delay
        clear_scl
        i2c_delay
        ret
; ---------------------------------------------------------

; write an array to an i2c slave --------------------------
; R4: counter
; R5: tmp
; R6: counter
; R7: tx ptr
i2c_write:
        mov.w   #i2c_tx_buf, R7 ; load pointer to tx buffer
        mov.w   &i2c_len, R6    ; start counter

        mov.b   #0, &i2c_mode   ; write mode
        call    #i2c_tx_start   ; send start condition + slave addr

        mov.b   &i2c_reg_addr, &i2c_byte
        call    #i2c_tx_byte    ; send register address within slave

write_loop:
        mov.b   @R7+, &i2c_byte
        call    #i2c_tx_byte

        dec.w   R6
        jnz     write_loop

        call    #i2c_tx_stop    ; send stop condition
        ret
; ---------------------------------------------------------

; read an array from an i2c slave -------------------------
; R4: counter
; R5: tmp
; R6: counter
; R7: tx ptr
i2c_read:
        mov.w   #i2c_rx_buf, R7 ; load pointer to tx buffer
        mov.w   &i2c_len, R6    ; start counter

        mov.b   #0, &i2c_mode   ; write mode
        call    #i2c_tx_start   ; send start condition + slave addr

        mov.b   &i2c_reg_addr, &i2c_byte
        call    #i2c_tx_byte    ; send register address within slave

        ; skip repeated start if nack received
        tst.b   &i2c_nack
        jnz      end_rx

        mov.w   #1, &i2c_mode   ; read mode
        call    #i2c_tx_start   ; repeated start

read_loop:
        cmp.w   #1, R6
        jnz     not_last_byte
        mov.b   #1, &i2c_end_rx
        
not_last_byte:
        call    #i2c_rx_byte
        mov.b   &i2c_byte, 0(R7)
        inc.w   R7

        dec.w   R6
        jnz     read_loop

        call    #i2c_tx_stop    ; send stop condition

end_rx:
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
        
        mov.b   #0x00, &i2c_reg_addr
        mov.w   #7, &i2c_len
        call    #i2c_write

main_loop:

        mov.b   #0x00, &i2c_reg_addr
        mov.w   #7, &i2c_len
        call    #i2c_read
        
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

        jmp     main_loop
        ret

;------------------------------------------------------------------------------
; MEMORY ALLOCATION
;------------------------------------------------------------------------------

        .section .bss
i2c_mode:
        .skip 1
i2c_byte:
        .skip 1
i2c_sda_rx:
        .skip 1
i2c_nack:
        .skip 1
i2c_end_rx:
        .skip 1
i2c_reg_addr:
        .skip 1
i2c_len:
        .skip 2
i2c_rx_buf:
        .skip 32

        .section .data
i2c_slave_addr:
        .byte 0x6F      ; MCP7940N slave address
i2c_tx_buf:
        .byte 0x80, 0x18, 0x14, 0x02, 0x24, 0x08, 0x26

;------------------------------------------------------------------------------
; END OF CODE
;------------------------------------------------------------------------------
