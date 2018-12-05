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

#include <stdlib.h>

#define _STM8HAL_INTERNAL
#include "stm8hal.h"
#include "random.h"

#ifdef _STM8_TESTS
#include <stdio.h>
#endif

////////////////////////////////////////////////////////////////////////////////
//
// pre-seed the PRNG at compile time, if not configured otherwise
//
// IDEA: Take initial seed from unique ID?

const char _STM8_T[] = __TIME__ ; // "HH:MM:SS"
const char _STM8_D[] = __DATE__ ; // "Mmm dd yyyy", for example "Oct 30 2010"

// extract some bits from ASCII data, '0'..'9' are in the range 0x30..0x39
// so we are mostly interested in the last 3-4 bits
const uint8_t _stm8_b0 = 0xFF & (
      ( *reinterpret_cast<const char*>( _STM8_T + 3 ) << 5 )    // m1, 3   bits
    ^ ( *reinterpret_cast<const char*>( _STM8_T + 6 ) << 2 )    // s1, 3   bits
    ^ ( *reinterpret_cast<const char*>( _STM8_D + 4 ) << 0 ) ); // d1, 2   bits
const uint8_t _stm8_b1 = 0xFF & (
      ( *reinterpret_cast<const char*>( _STM8_D + 10) << 7 )    // Y4, 1   bits
    ^ ( *reinterpret_cast<const char*>( _STM8_D + 1 ) << 4 )    // M2, 3   bits
    ^ ( *reinterpret_cast<const char*>( _STM8_T + 1 ) << 0 ) ); // h2, 3.5 bits
const uint8_t _stm8_b2 = 0xFF & (
      ( *reinterpret_cast<const char*>( _STM8_T + 7 ) << 4 )    // s2, 3.5 bits
    ^ ( *reinterpret_cast<const char*>( _STM8_T + 4 )      ) ); // m2, 3.5 bits
const uint8_t _stm8_b3 = 0xFF & (
      ( *reinterpret_cast<const char*>( _STM8_T + 0 ) << 6 )    // h1, 1.5 bits
    ^ ( *reinterpret_cast<const char*>( _STM8_D + 5 ) << 3 )    // d2, 3   bits
    ^ ( *reinterpret_cast<const char*>( _STM8_D + 2 ) << 0 ) ); // M3, 3   bits

// let's make some more mess
const uint16_t _stm8_w0 = 0xFFFF &  ( ( __LINE__ << 3 )
  ^  (_stm8_b0 << 10)  ^ ~(_stm8_b1 << 8)
  ^  (_stm8_b2 << 2)   ^ ~(_stm8_b3 << 0) );
const uint16_t _stm8_w1 = 0xFFFF & ~( ( __LINE__ << 9 )
  ^  (_stm8_b3 << 10)  ^ ~(_stm8_b2 << 8)
  ^  (_stm8_b1 << 2)   ^ ~(_stm8_b0 << 0) );
//

#ifndef _STM8_NO_PRESEED
const uint16_t _stm8_pre16_0 = (uint16_t)( 0xDEAD ^ _stm8_w0 );
const uint16_t _stm8_pre16_1 = (uint16_t)( 0xBEEF ^ _stm8_w1 );
const uint8_t  _stm8_pre8_0  = (uint8_t) ( 0xDE   ^ _stm8_w0 );
const uint8_t  _stm8_pre8_1  = (uint8_t) ( 0xAD   ^ _stm8_w0 >> 8);
const uint8_t  _stm8_pre8_2  = (uint8_t) ( 0xBE   ^ _stm8_w1 );
const uint8_t  _stm8_pre8_3  = (uint8_t) ( 0xEF   ^ _stm8_w1 >> 8);
#else
// in compliance with the C standard, initialize as if srand(1) had been called
// however, this would cause a 'slow' start for XORSHIFT type algorithms
const uint16_t _stm8_pre16_0 = (uin16_t) ( 0xDEAD ^ 1 );
const uint16_t _stm8_pre16_1 = (uin16_t) ( 0xBEEF ^ 1 );
const uint8_t  _stm8_pre8_0  = (uint8_t) ( 0xDE   ^ 1 );
const uint8_t  _stm8_pre8_1  = (uint8_t) ( 0xAD   ^ 1 );
const uint8_t  _stm8_pre8_2  = (uint8_t) ( 0xBE   ^ 1 );
const uint8_t  _stm8_pre8_3  = (uint8_t) ( 0xEF   ^ 1 );
#endif //_STM8_NO_PRESEED

////////////////////////////////////////////////////////////////////////////////

_EXTERN_C

////////////////////////////////////////////////////////////////////////////////
//
// XORSHIFT 16x2
//

// These must never be both zero, and ideally have a good distribution of 0s and 1s.
extern TINY uint16_t _xor16[2] = { _stm8_pre16_0, _stm8_pre16_1 };

OPTIMIZE_SIZE
void _STM8_F(xorshift16x2_seed)(uint16_t x, uint16_t y)
{
  _xor16[0] = x;
  _xor16[1] = y;
}

// We use the (8, 3, 9) parameters for the following permutations:
// t = x ^ (x<<8); x = y ; y = (y^(y>>9)) ^ (t^(t>>3))
#ifdef _STM8_TESTS
const uint8_t _XOR16_A = 8;
const uint8_t _XOR16_B = 3;
const uint8_t _XOR16_C = 9;
#endif // _STM8_TESTS

REQUIRED(_xor16)
NO_OPTIMIZE
NO_INLINE
uint16_t _STM8_F(xorshift16x2)(void)   // 8+22 cycles
{
  // A will magically pop out again at the end
  asm("LD    XH, A                       \n");

      // t = (x<<8) ^ x;
  asm("LD    A, s:_xor16+1               \n"     //        t.lo = x.lo
      "LD    XL, A                       \n"     // (temp)   XL = t.lo
      "XOR   A, s:_xor16+0               \n"     //        t.hi = x.lo ^ x.hi
      "LD    s:_xor16+0, A               \n"     // (temp) x.hi = t.hi
                                                 // (temp) x.lo = t3.lo
      // t3 = t >> 3; 16-bit, shifts using carry
      "SRL   A                           \n"
      "RRC   s:_xor16+1                  \n"
      "SRL   A                           \n"
      "RRC   s:_xor16+1                  \n"
      "SRL   A                           \n"
      "RRC   s:_xor16+1                  \n"

      // y = t ^ t3 ^ y, high bytes
      "XOR   A, s:_xor16+0               \n"     // t.hi ^ t3.hi
      "XOR   A, s:_xor16+2               \n"     //      ^ y.hi
      "MOV   s:_xor16+0, s:_xor16+2      \n"     // (final) x.hi = y.hi

      // yt.lo = y.hi >> 1   (thats all of y >> 9!!)
      "SRL   s:_xor16+2                  \n"     // (temp)  y.hi = yt.lo;

      // y = t ^ t3 ^ y, low bytes
      "EXG   A, XL                       \n"     // XL = new_y.hi; A = t.lo
      "XOR   A, s:_xor16+1               \n"     // t.lo ^ t3.lo
      "XOR   A, s:_xor16+2               \n"     //      ^ yt.lo
      "XOR   A, s:_xor16+3               \n"     //      ^ y.lo
      "MOV   s:_xor16+1, s:_xor16+3      \n"     // (final) x.lo = y.lo

      "RLWA  X, A                        \n"     //         X = new_y
      "LDW   s:_xor16+2, X               \n");   // (final) y = X

  // if A was saved originally, it has been restored by now
}

////////////////////////////////////////////////////////////////////////////////
//
// XORSHIFT8x4
//

// These must never be both zero, and ideally have a good distribution of 0s and 1s.
extern TINY uint8_t _xor8[4] = { _stm8_pre8_0, _stm8_pre8_1, _stm8_pre8_2, _stm8_pre8_3};

OPTIMIZE_SIZE
ALWAYS_INLINE
void _STM8_F(xorshift8x4_seed)(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3)
{
  _xor8[0] = s0; _xor8[0] = s1; _xor8[0] = s2; _xor8[0] = s3;
}

#ifdef _STM8_TESTS
const uint8_t _XOR8_A = 1;
const uint8_t _XOR8_B = 1;
const uint8_t _XOR8_C = 3;
#endif // _STM8_TESTS

// t = x ^ (x<<8); x = y ; y = (y^(y>>9)) ^ (t^(t>>3))
REQUIRED(_xor8)
// This routine implements the (1, 1, 3) triplet of permutations:
// t=(x^(x<<a)); x=y; y=z; z=v; return v=(v^(v>>c))^(t^(t>>b));
// ASM 15cy; C: 17cy; plus 8cy call overhead if not inline
OPTIMIZE_SPEED
uint8_t _STM8_F(xorshift8x4)(void)  //      (1, 1, 3)
{
  register uint8_t A;

         A   = _xor8[0];        //      LD    A, x
         A <<= 1; //a           //      SLA   A
         A  ^= _xor8[0];        //      XOR   A, x
  _xor8[0]   = A;               //      LD    ?b, A

         A >>= 1; //b           //      SRL   A
         A  ^= _xor8[0];        //      XOR   A, ?b

  _xor8[0]   = _xor8[1];        //      MOV
  _xor8[1]   = _xor8[2];        //      MOV
  _xor8[2]   = _xor8[3];        //      MOV

         A  ^= _xor8[3];        //      XOR   A, v(==z)
  _xor8[3] >>= 3; //c           //      SRL   v; SRL v; SRL v   TODO: STALLS pipeline!!
         A  ^= _xor8[3];        //      XOR   A, v
  _xor8[3]   = A;               //      LD    w, A

  return A;                     //      RET
}

#if 0

/*
static uint16_t ADC_GetConversionValue(void)
{
  uint16_t temph = 0;
  uint8_t templ = 0;

  if (ADC_(CR2_ALIGN)) // Right alignment, i.e. data is to be returned as bits ______9876543210
  {
    templ = ADC_(DRL); // Read LSB first, 8 bits, 76543210
    temph = ADC_(DRH); // Then read MSB, 2 bits, ______98
    temph = (uint16_t)(templ | (uint16_t)(temph << (uint8_t)8));
    // I could do LDW X, &ADC1_DRH
  }
  else // Left alignment, data is to be returned as bits 9876543210______
  {
    temph = ADC_(DRH); // Read MSB first 8 bits, 98765432
    templ = ADC_(DRL); // Then read LSB, 2 bits ______10
    temph = (uint16_t)((uint16_t)((uint16_t)templ << 6) | (uint16_t)((uint16_t)temph << 8));
  }

  return ((uint16_t)temph);
}
*/

static
uint16_t adc(uint8_t channel)
{
  ADC_(CSR_CH)    = channel;
  ADC_(CR2_ALIGN) = 1;          // use the least significant bits to return the 10 bit result
                                // this also means we can use standard access order to retrieve 16 bit result value
  ADC_(CR1_ADON)  = 1;          //start conversion, same bit as used for powerup...
  while(!ADC_(CSR_EOC));        // wait for End Of Conversion
  ADC_(CSR_EOC) = 0;            // clear flag
  return *((uint16_t*)&ADC_(DRH));
}

OPTIMIZE_SIZE
NO_INLINE
void _STM8_F(random_seed)()
{

  if(!ADC_(CR1_ADON))
  {
    //ADC_(CR1_SPSEL) = 0;      // SPSEL must be changed when ADC is in power down due to a clock glitch
    ADC_(CR1_ADON)  = 1;        // powerup; ADC needs one conversion time tCONV-14 cycles to warm up
    __delay_cycles(14);
  }
  // we're assuming ADC_[H|L]TR[H|L] are on their reset values so ADC_CSR_AWD wont be set
  //ADC_(CR1_CONT)  = 0;    // single mode

  for(uint8_t i=0; i<16; ++i)
    srand( adc(i) );

  ADC_(CR1_ADON) = 0; // powerdown

}

#endif

_END_EXTERN_C

////////////////////////////////////////////////////////////////////////////////
//
// TESTING
//

#ifdef _STM8_TESTS
#ifdef __cplusplus

// Note that although XORSHIFT PRNGs with multiple seeds return 0, the are
// slightly biased to other numbers: in the 2^(K*N)-1 period, every number
// appears 2^N times, except that 0 only appears 2^N-1 times.
// appears 2^K
// see Marsaglia paper for suggested parameters
// For UINT with N bits, the period is 2^(K*N)-1
// Good and fast candidiates:
//  4x 8: (1, 1, 3) or (3, 1, 1)
//  2x16: (8, 3, 9)
//  2x32: (10, 13, 10), (8, 9, 22), (2, 7, 3), (23, 3, 24) by Marsgalia
//  3x32: (10, 5, 26), (13, 19, 3), (1, 17, 2), (10, 1, 26) by Marsaglia
//  4x32: (5, 14, 1), (15, 4, 21), (23, 24, 3), (5, 12, 29) by Marsaglia
//  5x32: (2, 1, 4), (7, 13, 6), (1, 1, 20)
template< uint8_t K, typename UINT, uint8_t A, uint8_t B, uint8_t C>
struct XorShift
{
  UINT _seeds[K]; // x, y, z, w, v, ...
  XorShift(const UINT (&seeds)[K] )
  { for( uint8_t i=0; i<K; ++i) _seeds[i] = seeds[i]; }
  XorShift(UINT s )
  { for( uint8_t i=0; i<K; ++i) _seeds[i] = s; }
  UINT operator()()
  {
    UINT t = ( _seeds[0] ^ ( _seeds[0] << (UINT)A ) );
    for( uint8_t i=0; i<(K-1); ++i)     // use a circular table
      _seeds[i] = _seeds[1+i];          // for larger values of K
    _seeds[K-1] = ( _seeds[K-1] ^ ( _seeds[K-1] >> (UINT)C ) ) ^ ( t ^ ( t >> (UINT)B ) );
    return _seeds[K-1];
  }
};

// NOTE that XORSHIFT PRNGs that return the full state never return 0 !!
// see Marsaglia paper for suggested parameters
// For 64bit: (13, 7, 17) was recommended by Marsaglia
// For 32bit: (13, 17, 5) is Marsaglia's 'favourite'; expensive on STM8
// For 16bit: (7, 9, 8) ia a good and fast candidate
// For  8bit: (7,5,3) gives the full period
// All available full period 8 and 16 bit triplets are listed here:
//   http://www.arklyffe.com/main/2010/08/29/xorshift-pseudorandom-number-generator/
// Marsaglia lists all full period triplets for 32bit and 64 bit here:
//   http://www.jstatsoft.org/v08/i14/paper
template< typename UINT, uint8_t A, uint8_t B, uint8_t C > // K=1
struct XorShift< 1, UINT, A, B, C >
{
  UINT y; // seeds[1]
  inline XorShift(UINT seed=1) : y(seed) {}
  UINT operator()()
  {
    // Possible permutations:
    //  yˆ=y<<a; yˆ=y>>b; yˆ=y<<c;
    //  yˆ=y<<c; yˆ=y>>b; yˆ=y<<a;
    //  yˆ=y>>a; yˆ=y<<b; yˆ=y>>c;
    //  yˆ=y>>c; yˆ=y<<b; yˆ=y>>a;
    //  yˆ=y<<a; yˆ=y<<c; yˆ=y>>b;
    //  yˆ=y<<c; yˆ=y<<a; yˆ=y>>b;
    //  yˆ=y>>a; yˆ=y>>c; yˆ=y<<b;
    //  yˆ=y>>c; yˆ=y>>a; yˆ=y<<b;
    y ^= ( y << (UINT)A );
    y ^= ( y >> (UINT)B );
    y ^= ( y << (UINT)C );
    return y;
  }
};

NO_INLINE
OPTIMIZE_SPEED
bool _stm8_tests_random()
{
  // xorshift16x2
  {
    XorShift< 2, uint16_t,
      _XOR16_A, _XOR16_B, _XOR16_C > rng16( _xor16 );

    for( uint16_t i=0; i<1024; ++i)
    {
      register uint16_t r1 = _STM8_F(xorshift16x2)();
      register uint16_t r2 = rng16();
      if( r1 != r2 )
      {
        puts("STM/Tests/Random: xorshift16x2 sequence failed.");
        return false;
      }
    }
    putchar('\n');
  }

  // xorshift8x4
  {
    XorShift< 4, uint8_t,
      _XOR8_A, _XOR8_B, _XOR8_C> rng8( _xor8 );

    for( uint16_t i=0; i<10; ++i)
    {
      register uint16_t r1 = _STM8_F(xorshift8x4)();
      register uint16_t r2 = rng8();
      if( r1 != r2 )
      {
        puts("STM/Tests/Random: xorshift8x4 sequence failed.");
        return false;
      }
    }
    putchar('\n');

  }
  return true;
}

#endif // __cplusplus
#endif // #ifdef _STM8_TESTS

////////////////////////////////////////////////////////////////////////////////
//
//  DOCUMENTATION & RESEARCH NOTES
//

#if 0

//  DATE: August 31, 2010
//  FROM: Brad Forschinger
//  SUBJECT: 16 bit xorshift rng (now with more period) [0]
//
//  So, in Marsaglia's paper[1], the basic xorshift [2] rng the code is:
//
//      yˆ=(y<<a); y^=(y>>b); return (yˆ=(y<<c));
//
//  I looked at this in my last post, and had a brief look at the (a,b,c) triplets
//  to use. However, this only yields a period length of the word size used. In my
//  case, that's 2^16-1 -- not much. However, further on in the paper, section 3.1,
//  a method is described for longer periods through additional state and promotion:
//
//      t=(xˆ(x<<a)); x=y; return y=(yˆ(y>>c))ˆ(tˆ(t>>b));
//
//  This is listed as 2^64-1 using 32-bit words, but there's no reason we can't use
//  16-bit words. This reduces the period to 2^32-1, and requires new triplets.
//  I scanned (1,1,1) through till (15,15,15), here's the list:
//
//  (1,1,7)     (1,1,12)    (1,1,13)    (2,5,8)	    (2,5,13)
//  (2,13,15)   (2,15,13)   (3,7,6)     (5,3,1)*    (5,3,8)
//  (5,3,13)*   (5,7,4)*    (6,3,8)*    (7,1,6)	    (7,1,15)
//  (7,2,1)     (8,3,9)*    (9,14,5)    (11,8,5)*   (13,12,3)
//  (14,1,15)   (15,10,1)
//
//  Values marked with an asterisk score well on the majority of diehard tests.
//  It's probably not a good idea to use the other values, even though they do
//  have a complete period. Notably, all values fail binary matrices tests for
//  31x31 and  32x32 - I think this is expected, and mentioned in Marsaglia2003.
//
//  So, the routine in C:
//
//      uint16_t rnd_xorshift_32()
//      {
//        static uint16_t x=1,y=1;
//        uint16_t t=(x^(x<<5));
//        x=y;
//        return y=(y^(y>>1))^(t^(t>>3));
//      }
//
//  [0] http://b2d-f9r.blogspot.com/2010/08/16-bit-xorshift-rng-now-with-more.html
//  [1] http://www.jstatsoft.org/v08/i14/paper
//  [2] https://en.wikipedia.org/wiki/Xorshift
//
////////////////////////////////////////////////////////////////////////////////////
//
//  FROM: Jens
//
//  Looking at the (8,3,9) triplet, the code for t and y is:
//
//    t = x ^ (x<<8); x = y; y = (y^(y>>9)) ^ (t^(t>>3))
//
//  further simplifying y (order of XOR operations doesn't matter):
//
//    t3 = t>>3
//    yt = y>>9 = (y>>8)>>1;
//    y ^= t ^ t3 ^ yt
//
//  In terms of hi/lo bytes, assuming little endian:
//
//    t.lo  = x.lo
//    t.hi  = x.lo ^ x.hi
//    t3 = t>>3            ; will be implemented using 16bit shifts
//    x.*   = y.*
//    yt.lo = y.hi >> 1
//    y.lo ^= t.lo ^ t3.lo ^ yt.lo
//    y.hi ^= t.hi ^ t3.hi
//
//  Some corrections to the original parameter, additional tests and
//  configurations in particular for Type II generators can be found in
//  L'Ecuyer's paper here:
//
//    https://www.iro.umontreal.ca/~lecuyer/myftp/papers/xorshift.pdf
//
//  Some more tiny xorshift generator resources:
//
//
//  Marsaglia XORSHIFT Type 3, 4x 8bit parameters providing full 2^32-1 period:
// (1,3,1,2) (1,3,2,3) (1,4,2,5) (1,6,2,3) (1,6,7,3) (1,7,1,3) (1,7,3,5)
// (2,1,1,1) (3,1,1,1) (3,5,2,5) (3,5,4,5) (3,5,7,5) (3,6,2,1) (3,6,5,1)
// (4,5,1,6) (5,3,2,3) (6,2,1,5) (6,3,3,1) (6,3,7,1) (6,3,7,4) (7,1,3,5)
// (7,3,4,3) (7,5,3,2) (7,7,2,1) (7,7,4,1) (7,7,6,1)

#endif

