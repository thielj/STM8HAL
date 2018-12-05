#include "vregs.inc"

#define _STM8HAL_INTERNAL
#include "stm8hal.h"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        MODULE  delay__cycles_variable
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        PUBLIC  _STM8_F(delay_cycles)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        ; define a CODE NOROOT section with 2^2 alignment
        SECTION `.near_func.text`:CODE:NOROOT(2)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; extern "C" void delay_cycles(uint16_t wait_cycles);


;align4
_STM8_F(delay_cycles):
        PUSHW   X               ; 2cy

        SRLW    X               ; 1cy   // add a cycle if bit0 was set
        JRC     _b0             ; 1/2cy // 2 cycles for the jump, 1 if not taken
;align4
_b0:
        SUBW    X, #12          ; 2+1cy   ; adjust for call overhead, admin, etc, -24 cycles

        SRLW    X               ; 1cy   ; add two cycles when bit 0 was set
;align4 - 1cy fetch??
        JRC     _b1a
_b1a:   JRC     _loop

_loop:
;align4 - 1cy fetch??
        NOP                     ; 1cy   // burn 4 cycles each loop
        DECW    X               ; 1cy
        JRPL    _loop           ; 1/2cy
;align4 - 1cy fetch??
        POPW    X               ; 2cy
        RET
        ;NOP
        ;NOP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        END

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
