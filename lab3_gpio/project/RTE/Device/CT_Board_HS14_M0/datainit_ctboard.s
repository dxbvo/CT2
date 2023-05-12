;* ------------------------------------------------------------------
;* --  _____       ______  _____                                    -
;* -- |_   _|     |  ____|/ ____|                                   -
;* --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
;* --   | | | '_ \|  __|  \___ \   Zurich University of             -
;* --  _| |_| | | | |____ ____) |  Applied Sciences                 -
;* -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
;* ------------------------------------------------------------------
;* --
;* -- Project     : CT Board - Cortex M4
;* -- Description : Data Segment initialisation.
;* -- 
;* -- $Id$
;* ------------------------------------------------------------------


; -------------------------------------------------------------------
; -- __Main
; ------------------------------------------------------------------- 

                AREA    |.text|, CODE, READONLY

                IMPORT main
                IMPORT __scatterload
                EXPORT __main

__main          PROC
    
                ; initialize RW, ZI, C++ static init
                BL __scatterload
                ; call user main function
                LDR     R0, =main
                BX      R0
                ENDP

                ; __user_setup_stackheap is required by __scatterload
                ; we do not provide any heap: no C malloc nor C++ new usable
                IMPORT Stack_Size  [WEAK]
                IMPORT Stack_Mem   [WEAK]
                EXPORT __user_setup_stackheap
__user_setup_stackheap PROC
                ; fingerprint stack section
                LDR    R0, =Stack_Mem
                LDR    R1, =Stack_Size
                LDR    R2, =0xEFBEADDE  ; stack fingerprint (little endian!)
LoopStack       STR    R2, [R0]
                ADDS   R0, R0, #4
                SUBS   R1, #4
                BNE    LoopStack
                BX     LR 
                ENDP

; -------------------------------------------------------------------
; -- End of file
; -------------------------------------------------------------------  

                ALIGN
                    
                END
