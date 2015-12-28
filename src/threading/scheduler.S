
/*
 * scheduler.S
 *
 * Created: 18/12/2015 21:26:14
 *  Author: jcahier
 */ 

 #include <avr/io.h>

.data

.extern current_thread_id
.extern next_thread_id
.extern thread_ctx_ptrs

; struct thread_ctx offset definitions
__struct_thread_ctx_regs   = 0
__struct_thread_ctx_PC     = 32
__struct_thread_ctx_SP     = 34
__struct_thread_ctx_stack  = 36
__struct_thread_ctx_SREG   = 38



.text



;.global TIMER0_COMPA_vect
;TIMER0_COMPA_vect:
.global save_context
save_context:
        ; ========== SAVE USEFUL REGS TO THE STACK =========
        ; ***** save used regs
        push r0                     ; SP - 1
        push r17                    ; SP - 2
        push r18                    ; SP - 3
        ; ***** save SREG
        in   r17, _SFR_IO_ADDR(SREG)
        push r17                    ; SP - 4
        ; ***** get SP & save old value on the stack
        in   r17, _SFR_IO_ADDR(SPL)
        in   r18, _SFR_IO_ADDR(SPH)
        ; restore old value
        subi r17, lo8(-4) ; SP to be restored just before returning when switching back
        ; propagate carry to high byte
        sbci r18, hi8(-4)
        ; save computed SP value
        push r17                    ; SP - 5
        push r18                    ; SP - 6
        ; ***** save Z
        push r30                    ; SP - 7
        push r31                    ; SP - 8

        ; =========== GET ADRESS OF CURRENT THREAD CONTEXT ==========
        ; ***** Z = &( *thread_ctx_ptrs[current_thread_id]
        ; prepare r30,r31 as a "complement offset"
        ; r30 <- (current_thread_id)
        lds  r30, current_thread_id
        ; r31 <- 0 ; address high byte
        ldi  r31, 0
        ; r30 <- r30<<1
        lsl  r30
        ; r31 <- r31<<1 + C
        rol  r31

        ; Z <- thread_ctx_ptrs[current_thread_id]
        ;    == ((thread_ctx*)current_thread_id)<<1 - (-thread_ctx_ptrs)
        subi r30, lo8(-(thread_ctx_ptrs))
        sbci r31, hi8(-(thread_ctx_ptrs))

        ; Z <- r17,r18 <- *thread_ctx_ptrs[current_thread_id]
        ld   r17, Z
        ldd  r18, Z+1
        mov  r30, r17
        mov  r31, r18

        ; ============= MOVE MCU STATE INTO CURRENT THREAD CONTEXT ============
        ; ***** Save all stacked regs into context
        pop  r17                    ; (r31)  ; SP - 7
        std  Z+__struct_thread_ctx_regs+31, r17
        pop  r17                    ; (r30)  ; SP - 6
        std  Z+__struct_thread_ctx_regs+30, r17
        pop  r17                    ; (SPH)  ; SP - 5
        std  Z+__struct_thread_ctx_SP+1, r17
        pop  r17                    ; (SPL)  ; SP - 4
        std  Z+__struct_thread_ctx_SP, r17
        pop  r17                    ; (SREG) ; SP - 3
        std  Z+__struct_thread_ctx_SREG, r17
        pop  r17                    ; (r18)  ; SP - 2
        std  Z+__struct_thread_ctx_regs+18, r17
        pop  r17                    ; (r17)  ; SP - 1
        std  Z+__struct_thread_ctx_regs+17, r17
        pop  r17                    ; (r0)   ; SP
        std  Z+__struct_thread_ctx_regs+0, r17

        ; **** Save other registers into thread_ctx
        ;std  Z+__struct_thread_ctx_regs+0, r0
        std  Z+__struct_thread_ctx_regs+1, r1
        std  Z+__struct_thread_ctx_regs+2, r2
        std  Z+__struct_thread_ctx_regs+3, r3
        std  Z+__struct_thread_ctx_regs+4, r4
        std  Z+__struct_thread_ctx_regs+5, r5
        std  Z+__struct_thread_ctx_regs+6, r6
        std  Z+__struct_thread_ctx_regs+7, r7
        std  Z+__struct_thread_ctx_regs+8, r8
        std  Z+__struct_thread_ctx_regs+9, r9
        std  Z+__struct_thread_ctx_regs+10, r10
        std  Z+__struct_thread_ctx_regs+11, r11
        std  Z+__struct_thread_ctx_regs+12, r12
        std  Z+__struct_thread_ctx_regs+13, r13
        std  Z+__struct_thread_ctx_regs+14, r14
        std  Z+__struct_thread_ctx_regs+15, r15
        std  Z+__struct_thread_ctx_regs+16, r16
        ;std  Z+__struct_thread_ctx_regs+17, r17
        ;std  Z+__struct_thread_ctx_regs+18, r18
        std  Z+__struct_thread_ctx_regs+19, r19
        std  Z+__struct_thread_ctx_regs+20, r20
        std  Z+__struct_thread_ctx_regs+21, r21
        std  Z+__struct_thread_ctx_regs+22, r22
        std  Z+__struct_thread_ctx_regs+23, r23
        std  Z+__struct_thread_ctx_regs+24, r24
        std  Z+__struct_thread_ctx_regs+25, r25
        std  Z+__struct_thread_ctx_regs+26, r26
        std  Z+__struct_thread_ctx_regs+27, r27
        std  Z+__struct_thread_ctx_regs+28, r28
        std  Z+__struct_thread_ctx_regs+29, r29
        ;std  Z+__struct_thread_ctx_regs+30, r30
        ;std  Z+__struct_thread_ctx_regs+31, r311

        ; ***** get PC from (SP);(SP-1) and save in it context
        ; hi8(PC)
        pop  r17
        std  Z+__struct_thread_ctx_PC+1, r17
        ; lo8(PC)
        pop  r18
        std  Z+__struct_thread_ctx_PC, r18

        ; ============= SAVE STACK INTO CURRENT THREAD CONTEXT =============
        ; X <- SP
        ldd  r26, Z+__struct_thread_ctx_SP
        ldd  r27, Z+__struct_thread_ctx_SP+1
        ; Y <- stack
        ldd  r28, Z+__struct_thread_ctx_stack
        ldd  r29, Z+__struct_thread_ctx_stack+1

        ; XXXXXXXXXXXXX STUB TO RETURN NORMALLY FROM save_context XXXXXXXXXXXXXXXX
        push r18 ; XXXXXXX REMOVE THIS                                         XXX
        push r17 ; XXXXXXX REMOVE THIS                                         XXX
        ; XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

    ; TODO : test stack allocation before copying  XXXXXX
    
    stack_copy_loop:
    ; do {copy_byte();}
        ld   r17, X+
        st   Y+ , r17
    ; while (X != RAMEND+1)
        ; test low_byte first
        cpi  r26, lo8(RAMEND+1) ; 'true' each 256 bytes
        brne stack_copy_loop
        ; then hibyte
        cpi  r27, hi8(RAMEND+1) ; end reached
        brne stack_copy_loop
    ; loop ended


        ; ===========================================================
        ; ===                   CONTEXT SWITCH                    ===
        ; ===========================================================

        ; now do the opposite,

;        reti
        ret
