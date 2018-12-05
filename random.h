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

#ifndef __STM8HAL_RANDOM_H
#define __STM8HAL_RANDOM_H

_EXTERN_C

////////////////////////////////////////////////////////////////////////////////
//
// XORSHIFT 16x2
//

// 16-bit XORSHIFT PRNG with 32-bit state, period length 2^32-1
//
// Not suitable for cryptography, but acceptable for almost everything else.
//
// The generator returns the full range of unsigned 16-bit integers, i.e.
// [0..UINT16_MAX], with a tiny bias for numbers other than zero (that is, in
// practical terms, over the full period length of 2^32-1, all values will occur
// 2^16 times, except for the value zero which will occur one time less).
//
// handcoded assembly, 8+22 cycles, 39 bytes code, 4 byte data
NO_INLINE
uint16_t _STM8_F(xorshift16x2)(void);

// These must never be both zero, and ideally have a good distribution
// of 0s and 1s. Using values like 0, 0xffff or -1 is not recommended.
//
// If _STM8_NO_PRESEED has been set during compilation, the seeds are
// initialized as if srand(1) had been called. Otherwise, a compile time
// seed is used to initialze the generator.
// NOTE: srand(1) does not actually set these to 1.
void _STM8_F(xorshift16x2_seed)(uint16_t x, uint16_t y);

////////////////////////////////////////////////////////////////////////////////
//
// XORSHIFT 8x4
//

// 8-bit XORSHIFT PRNG with 32-bit state, period length 2^32-1
//
// Not suitable for cryptography, but acceptable for almost everything else.
//
// The generator returns the full range of unsigned 8-bit integers, i.e.
// [0..255], with a tiny preference for numbers other than zero (that is, in
// practical terms, over the full period length of 2^32-1, all values will occur
// 2^24 times, except for the value zero which will occur one time less).
//
// inlined C, ~17 cycles, ?? bytes code, 4 byte data
uint8_t _STM8_F(xorshift8x4)(void);

// These must never be all zero, and ideally have a good distribution
// of 0s and 1s. Using values like 0, 0xff or -1 is not recommended.
//
// If _STM8_NO_PRESEED has been set during compilation, the seeds are
// initialized as if srand(1) had been called. Otherwise, a compile time
// seed is used to initialze the generator.
// NOTE: srand(1) does not actually set these to 1.
void _STM8_F(xorshift8x4_seed)(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3);

//OPTIMIZE_SPEED
//ALWAYS_INLINE
//inline uint8_t _STM8_F(rand8)(void)
//{
//    static TINY bool flag = 0;
//    static TINY NO_INIT uint8_t buf;
//    if( ( flag = !flag ) )
//    {
//      register uint16_t temp = _STM8_F(xorshift16x2)();
//      buf = ( temp >> 8 ) & 0xFF;
//      return (uint8_t) temp;
//    }
//    return buf;
//}



////////////////////////////////////////////////////////////////////////////////
//
// C STD COMPATIBLE REPLACEMENT FUNCTIONS
//

// Standard srand() function, using XORSHIFT with a period length of 2^32-1.
//
// The generator returns the full range of positive signed 16-bit integers, i.e.
// [0..32767], with a tiny bias for numbers other than zero (that is, in
// practical terms, over the full period length of 2^32-1, all values will occur
// 2^17 times, except for the value zero which will occur one time less).
//
// If _STM8_NO_PRESEED has been set during compilation, the seeds are initialized
// as if srand(1) had been called; i.e. both are set to 1. Otherwise, a compile
// time seed is used to initialze the generator.
OPTIMIZE_SPEED
ALWAYS_INLINE
inline int16_t _STM8_F(rand)(void)
{
  return (int16_t)( _STM8_F(xorshift16x2)() & 0x7FFF);
}

// These should be random values, i.e. with a good distribution of 0s and 1s.
// Using values like 0, 0xff or -1 is not recommended.
//
// If _STM8_NO_PRESEED has been set during compilation, the seeds are
// initialized as if srand(1) had been called. Otherwise, a compile time
// seed is used to initialze the generator.
// NOTE: srand(1) does not actually set these to 1.
OPTIMIZE_SPEED
ALWAYS_INLINE
inline void _STM8_F(srand)(uint16_t seed)
{
  _STM8_F(xorshift16x2_seed)( 0xDEAD^seed, 0xBEEF^seed);
}

OPTIMIZE_SPEED
ALWAYS_INLINE
inline int16_t _STM8_F(randr)( int16_t min, int16_t max )
{
    return (int16_t) _STM8_F(xorshift16x2)() % ( max - min + 1 ) + min;
}

////////////////////////////////////////////////////////////////////////////////
//
//  FUNCTIONS TO OBTAIN RANDOM SEEDS ("NOISE") FROM THE HARDWARE
//

// requires an MCU with ADC and should be called once before setting up pins
//void _STM8_F(random_seed)();

_END_EXTERN_C

////////////////////////////////////////////////////////////////////////////////
//
//  TESTING
//

#ifdef _STM8_TESTS
#ifdef __cplusplus
bool _stm8_tests_random();
#endif
#endif

#endif // __STM8HAL_RAND_H
