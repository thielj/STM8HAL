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
#include "crc.h"

_EXTERN_C

OPTIMIZE_SPEED
NO_INLINE
uint8_t _STM8_F(scale_add8)( uint16_t i, fract8 scale, uint16_t add)
{
  // arguments: X uint8_t, A fract8, Y uint16_t
  asm("PUSHW  X                 \n" // preserve register

      "LDW    s:VR+2, Y         \n"
      "MUL    X, A              \n"
      "ADDW   X, VR+2           \n" // ADDW doesnt support shortmem, but #imm
      "LD     A, XH             \n" // return high 8 bits

      "POPW   X                 \n");
}

_END_EXTERN_C
