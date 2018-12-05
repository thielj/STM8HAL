#include "vregs.inc"

#define _STM8HAL_INTERNAL
#include "stm8hal.h"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        MODULE  delay_cycles_fixed
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        PUBLIC  _STM8_F(delay_8cycles)
        PUBLIC  _STM8_F(delay_16cycles)
        PUBLIC  _STM8_F(delay_32cycles)
        PUBLIC  _STM8_F(delay_64cycles)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        ; define a CODE NOROOT section with 2^2 alignment
        SECTION `.near_func.text`:CODE:NOROOT(2)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; extern "C" void delay_64cycles();
; extern "C" void delay_32cycles();
; extern "C" void delay_16cycles();
; extern "C" void delay_8cycles();


_STM8_F(delay_256cycles):
        CALLR _STM8_F(delay_128cycles)
_STM8_F(delay_128cycles):
        CALLR _STM8_F(delay_64cycles)
_STM8_F(delay_64cycles):
        CALLR _STM8_F(delay_32cycles)
_STM8_F(delay_32cycles):
        CALLR _STM8_F(delay_16cycles)
_STM8_F(delay_16cycles):
        CALLR _STM8_F(delay_8cycles)
_STM8_F(delay_8cycles):
        RET
        ;NOP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

        END

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
