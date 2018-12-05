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

#ifndef __STM8HAL_DEBUG_H
#define __STM8HAL_DEBUG_H

////////////////////////////////////////////////////////////////////////////////
// Use TIM2/5 to count cycles.
// It doesn't hurt performance, but is only useful with the debugger attached.
//
#if defined(__ICCSTM8__)

# if defined(TIM2_OVR_UIF_vector)
#  define _STM8_HAVE_TIM2
#  define _STM8_CYCLES TIM2
# elif defined(TIM5_OVR_UIF_vector)
#  define _STM8_HAVE_TIM5
#  define _STM8_CYCLES TIM5
# else
#  error Couldn't find register information for TIM2 or TIM5.
# endif

# define _STM8_CYCLES_CNTRH       _GLUE( _STM8_CYCLES, _CNTRH )
# define _STM8_CYCLES_CNTRL       _GLUE( _STM8_CYCLES, _CNTRL )
# define _STM8_CYCLES_PSCR        _GLUE( _STM8_CYCLES, _PSCR)  // PSC 0x07
# define _STM8_CYCLES_ARRH        _GLUE( _STM8_CYCLES, _ARRH )
# define _STM8_CYCLES_ARRL        _GLUE( _STM8_CYCLES, _ARRL )
# define _STM8_CYCLES_CR1         _GLUE( _STM8_CYCLES, _CR1 )  // CEN 0x01

  // TODO: These are highly variable between the different lines
  // TIM4/6 always seem to use the same values. check for TIM 2/5
# define _STM8_CYCLES_CGR         CLK_PCKENR1    // clock gating register
# define _STM8_CYCLES_CGR_MASK    (1<<5)


#else // check for STM's system header defines

# if defined(TIM2)
#  define _STM8_HAVE_TIM2
#  define _STM8_CYCLES TIM2
# elif defined(TIM5)
#  define _STM8_HAVE_TIM5
#  define _STM8_CYCLES TIM5
# else
#  error Couldn't find register information for TIM2 or TIM5.
# endif

  // these can have slightly different addresses
# define _STM8_CYCLES_CNTRH       _GLUE( _STM8_CYCLES, ->CNTRH ) // @ 0x530C
# define _STM8_CYCLES_CNTRL       _GLUE( _STM8_CYCLES, ->CNTRL ) // @ 0x530D
# define _STM8_CYCLES_PSCR        _GLUE( _STM8_CYCLES, ->PSCR)   // PSC 0x07
# define _STM8_CYCLES_ARRH        _GLUE( _STM8_CYCLES, ->ARRH )
# define _STM8_CYCLES_ARRL        _GLUE( _STM8_CYCLES, ->ARRL )
# define _STM8_CYCLES_CR1         _GLUE( _STM8_CYCLES, ->CR1 )   // CEN 0x01

# define _STM8_CYCLES_CGR         CLK_PCKENR1
# define _STM8_CYCLES_CGR_MASK    (1<<5)

#endif

#pragma required=_STM8_CYCLES_CNTRH
#pragma required=_STM8_CYCLES_CNTRL
OPTIMIZE_SIZE
inline void enableCycleCounter()
{
  // Peripheral clock gating register
  _STM8_CYCLES_CGR |= _STM8_CYCLES_CGR_MASK;
  _STM8_CYCLES_PSCR = 0;        // no prescaler, i.e. fMaster/1
  _STM8_CYCLES_ARRH = 255;      // always access H first, L second.
  _STM8_CYCLES_ARRL = 255;
  _STM8_CYCLES_CR1 |= 0x01;     // CEN=0x01, Enable Timer
}

#endif  // __STM8HAL_DEBUG_H