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

#ifndef __STM8HAL_STM8HAL_H
#define __STM8HAL_STM8HAL_H

#if !defined(__STM8__)
 #error "Platform not supported or __STM8__ not defined"
#endif

#ifndef __IAR_SYSTEMS_ASM__

  #include <stdint.h>
  #include <stdbool.h>
//#include <string.h>
  #include <stddef.h>
//#include <assert.h>
  #include <stdlib.h>

  #include "compat.h"

#endif // #ifndef __IAR_SYSTEMS_ASM__

#if defined(STM8HAL_CONF)
  #include _STRINGIFY(STM8HAL_CONF)
#endif

#if defined(MCU)
  #if !defined(MCU_H)
    #define MCU_H _GLUE3(<io, MCU, .h>)
  #endif
#else
  #define MCU stm8
  #if !defined(MCU_H)
    #if !defined(_STM8_SIMULATOR_) && !defined(_STM8HAL_INTERNAL)
      #warning Using the generic STM8 family defaults; Please define MCU or MCU_H before targeting real hardware
    #endif
    #define MCU_H <iostm8.h>
  #endif
#endif

#include MCU_H

#ifndef F_CPU
  #if !defined(_STM8_SIMULATOR_) && !defined(_STM8HAL_INTERNAL)
    #warning F_CPU not defined, assuming F_CPU=16000000 (16MHz)
  #endif
  #define F_CPU 16000000
#endif

#ifndef _STM8_T
#define _STM8_T(name) _stm8_ ## name
#endif
#ifndef _STM8_F
#define _STM8_F(name) _stm8_ ## name
#endif

#ifndef NDEBUG
#define _STM8_TESTS     1
#endif


#ifndef __IAR_SYSTEMS_ASM__

  #define STM8HAL_NAMESPACE_BEGIN
  #define STM8HAL_NAMESPACE_END

  #ifndef yield
  #  define yield() do {} while(0)
  #endif

  #ifndef wdg
  #  define wdg()   do {} while(0)
  // define wdg()   do { IWDG_KR = 0xAA; } while(0)
  #endif

  // if a fract8 has the value "64", that should be interpreted
  //          as 64/256ths, or one-quarter.
  //  fract8   range is 0 to 0.99609375 in steps of 0.00390625
  //
  typedef                uint8_t fract8; ///< ANSI: unsigned short _Fract

  // data type defs
  typedef volatile const uint8_t RoReg; /**< Read only 8-bit register (volatile const unsigned int) */
  typedef volatile       uint8_t RwReg; /**< Read-Write 8-bit register (volatile unsigned int) */

  struct _STM8_T(reg32) {
      union {
        uint32_t value;
        uint32_t l[1];
        uint16_t w[2];
        uint8_t  b[4];
      };
  };

#endif


// IAR STM8 INLINE ASSEMBLY:
//
// The compiler seems to ignore the usual calling conventions
// with regards to scratch registers
// we must consider everything that is not already used for
// passing arguments as precious. As such, we need to define
// our own scratch registers
#if defined(__ICCSTM8__)
#define _STM8_SCRATCH_REGS 8

extern TINY NO_INIT //USED
volatile unsigned char VR[ _STM8_SCRATCH_REGS ];

#define _STM8_PREFER_PASS_X_REG 1
#endif

//#define _STM8_PREFER_C_INLINE 1



//#include "misc.h"

#endif // __STM8HAL_STM8HAL_H
