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

#ifndef __STM8HAL_DELAY_H
#define __STM8HAL_DELAY_H

_EXTERN_C

////////////////////////////////////////////////////////////////////////////////
//
// DELAY FUNCTIONS - CYCLES HARDCODED
//
// Note that due to pipelining, the actual number of cycles can be bigger.
// In particular, when returning to instructions overlapping a 32-bit boundary,
// additional fetch cycle(s) will be needed. It is recommended to factor this
// in and return to a NOP instruction instead.

extern void _STM8_F(delay_8cycles)();
extern void _STM8_F(delay_16cycles)();
extern void _STM8_F(delay_32cycles)();
extern void _STM8_F(delay_64cycles)();
extern void _STM8_F(delay_128cycles)();
extern void _STM8_F(delay_256cycles)();

////////////////////////////////////////////////////////////////////////////////
//
// DELAY FUNCTIONS - CYCLES VARIABLE
//

// 24 cycles minimum - use NOPs for shorter periods.
extern void _STM8_F(delay_cycles)(uint16_t wait_cycles);

////////////////////////////////////////////////////////////////////////////////
//
// DELAY FUNCTIONS - CYCLES VARIABLE, SPECIFIED IN MICRO SECONDS
//

// TODO: Calibrate !!!

// 16MHz, 16 cycles ~ 1us
NO_INLINE
void _STM8_F(delay_us_cycles8)(uint8_t wait_us);
NO_INLINE
void _STM8_F(delay_us_cycles16)(uint16_t wait_us);

////////////////////////////////////////////////////////////////////////////////
//
// DELAY FUNCTIONS - BASED ON TIMER / INTERRUPTS
//

NO_INLINE
void _STM8_F(delay_us16)(uint16_t wait_us);

NO_INLINE
void _STM8_F(delay_ms16)(uint16_t millis);

#ifndef delay
#define delay(millis)    _STM8_F(delay_ms16)(millis)
#endif

#ifndef delay_ms
#define delay_ms(millis) _STM8_F(delay_ms16)(millis)
#endif

#ifndef delay_us
#ifdef _STM8_SIMULATOR_
#define delay_us(wait_us) _STM8_F(delay_us_cycles16)(wait_us)
#else
#define delay_us(wait_us) _STM8_F(delay_us16)(wait_us)
#endif
#endif

#ifndef delayMicroseconds
#define delayMicroseconds(wait_us) delay_us(wait_us)
#endif

_END_EXTERN_C


#ifdef __cplusplus

////////////////////////////////////////////////////////////////////////////////
//
// With pipelining, returning or jumping to a 5 byte instruction
// will always stall the execution pipeline. In this case it is
// recommended to factor this in and reduce NOPs by 1 cycle.
// Any other instruction that is split over two long memory words
// can also cause a pipeline stall, unless it is prefixed with a
// NOP instruction.
#define min_(a,b) ( (a) > (b) ? (a) : (b) )
template<int16_t CYCLES>
__attribute__((always_inline))
inline void _STM8_F(nops)(bool cc=false)
{
  if( CYCLES > 64 ) // allow these to be followed by a single true NOP
  {
    _STM8_F(delay_64cycles)();
    _STM8_F(nops)< min_(0,CYCLES-64) >(cc);
  }
  else if( CYCLES > 32 ) // allow these to be followed by a single true NOP
  {
    _STM8_F(delay_32cycles)();
    _STM8_F(nops)< min_(0,CYCLES-32) >(cc);
  }
  else if( CYCLES > 16 ) // allow these to be followed by a single true NOP
  {
    _STM8_F(delay_16cycles)();
    _STM8_F(nops)< min_(0,CYCLES-16) >(cc);
  }
  else if( CYCLES > 9 ) // allow these to be followed by a single true NOP
  {
    _STM8_F(delay_8cycles)();
    _STM8_F(nops)< min_(0,CYCLES-8) >(cc);
  }
  else //if( CYCLES > 1)
  {
    const int16_t even = (CYCLES>>1)<<1;
    if(! cc) asm("PUSH CC");
    else asm("TNZW X");
    //else asm("NOP \n NOP");
    _STM8_F(nops)< min_(0,even-2) >(true);
    if(! cc) asm("POP CC");
    _STM8_F(nops)< CYCLES-even >(cc);
  }

}
template<> __attribute__((always_inline)) inline void _STM8_F(nops)<0>(bool) {}
template<> __attribute__((always_inline)) inline void _STM8_F(nops)<1>(bool) { nop(); }
#undef min_

#endif // __cplusplus

#endif // __STM8HAL_DELAY_H