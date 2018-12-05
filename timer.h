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

#ifndef __STM8HAL_TIMER_H
#define __STM8HAL_TIMER_H

////////////////////////////////////////////////////////////////////////////////
//
// microseconds should correspond to real time (i.e. 1000us per 1ms)
// but each millisecond is 2.4% longer, i.e. has 1024 real microseconds:
//  realSecondsElapsed = ourMillisElapsed >> 10;              // fast
//  realMillisElapsed = ( ourMillisElapsed * 1000 ) >> 10;    // expensive multiplication

extern TINY volatile _stm8_reg32 _stm8_time_elapsed;

// Fast version, ignoring pending timer interrupt
OPTIMIZE_SPEED
ALWAYS_INLINE
PURE
inline uint16_t _STM8_F(millis16)()
{
  return _stm8_time_elapsed.w[1];       // should be atomic
}

////////////////////////////////////////////////////////////////////////////////

_EXTERN_C

void _STM8_F(enable_timer)();

PURE NO_INTERRUPTS uint32_t _STM8_F(millis32)();
#ifndef millis
#define millis() _STM8_F(millis32)()
#endif

PURE NO_INTERRUPTS uint16_t _STM8_F(micros16)();
#ifndef micros
#define micros() _STM8_F(micros16)()
#endif

_END_EXTERN_C


#endif // __STM8HAL_TIMER_H
