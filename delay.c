/*******************************************************************************

Copyright (c) 2017-present J Thiel

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*******************************************************************************/

#define _STM8HAL_INTERNAL

#include "stm8hal.h"
#include "timer.h"
#include "delay.h"

////////////////////////////////////////////////////////////////////////////////
//
//  DELAY FUNCTIONS - BASED ON PROCESSOR CYCLES
//

OPTIMIZE_SIZE
NO_INLINE
void _STM8_F(delay_us_cycles8)(volatile uint8_t wait_us)
{
  // allow for call overhead on first execution
  // ldw x, #wait_us; call func; jra; loop; nop; skip; ...; decw x; jrne loop; ret
  do {
    nop();
    if( --wait_us == 0 ) return;
    _STM8_F(delay_8cycles)();
    nop(); nop(); nop();
  } while( 1);
}

OPTIMIZE_SIZE
NO_INLINE
void _STM8_F(delay_us_cycles16)(volatile uint16_t wait_us)
{
  _STM8_F(delay_us_cycles8)((uint8_t)wait_us);
  for(uint8_t w = wait_us >> 8; w; --w)
    _STM8_F(delay_us_cycles8)(255);
}

////////////////////////////////////////////////////////////////////////////////
//
//  DELAY FUNCTIONS - BASED ON TIMER / INTERRUPTS
//

OPTIMIZE_SIZE
NO_INLINE
void _STM8_F(delay_ms16)(uint16_t wait_ms)
{
  // TODO: feed the dog? wdg();
  // TODO: define a weak void yield();  ???
  const uint16_t start = _STM8_F(millis16)();
  do { /* nothing */ } while( ((uint16_t)( _STM8_F(millis16)() - start)) < wait_ms);
}

OPTIMIZE_SIZE
NO_INLINE
void _STM8_F(delay_us16)(uint16_t wait_us)
{
  // TODO: feed the dog? wdg();
  const uint16_t start = _STM8_F(micros16)();
  do { /* nothing */ } while( ((uint16_t)( _STM8_F(micros16)() - start)) < wait_us);
}

////////////////////////////////////////////////////////////////////////////////

//OPTIMIZE_SIZE
//ALWAYS_INLINE
//void _STM8_F(delay_cycles_wrapper)(volatile uint16_t wait_cycles)
//{
//  if( wait_cycles > 32 ) _STM8_F(delay_cycles_new)( wait_cycles - 32);
//  else
//  {
//    VR[1] = wait_cycles; // need to do about 10 less
//    nop();
//  }
//}

////OPTIMIZE_SIZE
////NO_INLINE
////void _STM8_F(delay_at_least_24_cycles)(volatile uint16_t wait_cycles)
////{
////  // 8+16=24 min
////  asm("PUSHW X          \n"
////
////      "SRLW  X          \n" // add a cycle if bit0 was set
////      "JRC   __bit0     \n" // 2 cycles for the jump, 1 if not taken
////      "__bit0:          \n"
////
////      "SUBW  X, #12     \n" // adjust for call overhead, admin, etc, -24 cycles
////
////      "SRLW  X          \n" // add two cycles when bit 0 was set
////      "JRC   __bit1a    \n"
////      "__bit1a:         \n"
////      "JRC   __bit1b    \n"
////      "__bit1b:         \n"
////
////      "__loop:          \n" // burn 4 cycles each loop
////      "NOP              \n"
////      "DECW  X          \n"
////      "JRPL __loop      \n"
////
////      "__restore:       \n"
////      "POPW  X          \n");
////}
////
////// TODO: merge the delay functions into a single assembly routine
////
////NO_OPTIMIZE
////NO_INLINE
////void _STM8_F(delay_8_cycles)() { asm("volatile:"); }
////
////NO_OPTIMIZE
////NO_INLINE
////void _STM8_F(delay_16_cycles)() { asm("CALLR $+2"); /* RET */ }
////
////NO_OPTIMIZE
////NO_INLINE
////void _STM8_F(delay_32_cycles)() { asm("CALLR $+2"); asm("CALLR $+2"); /* RET */ }
////
////NO_OPTIMIZE
////NO_INLINE
////void _STM8_F(delay_64_cycles)() { asm("CALLR $+2"); asm("CALLR $+2"); asm("CALLR $+2"); /* RET */ }
////
////NO_OPTIMIZE
////NO_INLINE
////void _STM8_F(delay_128_cycles)() { asm("CALLR $+2"); asm("CALLR $+2"); asm("CALLR $+2"); asm("CALLR $+2"); /* RET */ }
////
////NO_OPTIMIZE
////NO_INLINE
////void _STM8_F(delay_256_cycles)() { asm("CALLR $+2"); asm("CALLR $+2"); asm("CALLR $+2"); asm("CALLR $+2"); asm("CALLR $+2"); /* RET */ }
////
