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

#if defined(__ICCSTM8__)
//#pragma data_alignment=4
TINY NO_INIT //USED
volatile unsigned char VR[ _STM8_SCRATCH_REGS ];
#endif

MESSAGE( "STM8HAL v0.5, (c) J Thiel"
         ", MCU=" _STRINGIFY(MCU)
         ", MCU_H=" _STRINGIFY(MCU_H) )


#if 0
// IDEA: always have VR[0] = 0 to promote 8 to 16 bit numbers
// for round=0x80, use SWAPW, N=1 if result hast bit15 set?

// working on memory: MOV, CLR, INC, DEC, TNZ, CPL, NEG, SWAP, all shifts PUSH, POP  (EXG)
// indexed: LD, LDW, CLR, CP, INC, DEC, TNZ, CPL, NEG, SWAP, all shifts / rotates,
//  all 2 operand ops: AND, OR, XOR, ADC, ADD, SUB, SBC, ADDW, SUBW,
//  SLA, SLL, SRL, SRA, RLC, RRC

// (X/Y), (ptr, X/Y), some SP:  same cycles! Y prefix opcode
// [shortptr.w]   [longptr.w]     +3 cycles, indirect prefix opcode
// ([shortptr.w], X/Y/SP)
#endif
