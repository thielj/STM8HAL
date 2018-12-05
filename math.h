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

#ifndef __STM8HAL_MATH_H
#define __STM8HAL_MATH_H

////////////////////////////////////////////////////////////////////////////////
// Saturated add off two unsigned integers
// Generates optimal or close to optimal code on many platforms
template< typename UINT, UINT ceil = (UINT)-1>
ALWAYS_INLINE CONST
inline UINT _STM8_F(qadd)( UINT a, UINT b)
{
  UINT c = a + b;
  if( c < a ) /* can only happen due to overflow */
    c = ceil;
  return c;
}

// This generates optimal code equivalent to:
//  asm("ADD    A, s:?b0 \n  JRNC   __exit \n  LD A, #$FF \n");
ALWAYS_INLINE CONST
inline uint8_t _STM8_F(qadd8)( uint8_t a, uint8_t b)
{
  return _STM8_F(qadd)<uint8_t>( a, b);
}

// This generates optimal code equivalent to:
//  asm("ADD    A, s:?b0 \n  JRNC   __exit \n  LD A, #$FF \n");
ALWAYS_INLINE CONST
inline uint8_t _STM8_F(qadd7)( uint8_t a, uint8_t b)
{
  return _STM8_F(qadd)<uint8_t, 0x7F>( a, b);
}

template< typename UINT, UINT floor = 0>
ALWAYS_INLINE CONST
#pragma optimize=speed
inline UINT _STM8_F(qsub)( UINT a, UINT b)
{
  UINT c = a - b;
  if( c > a )  /* can only happen due to underflow */
    c = floor;
  return c;
}

// This generates almost optimal code
ALWAYS_INLINE CONST
inline uint8_t _STM8_F(qsub8)( uint8_t a, uint8_t b)
{
  return _STM8_F(qsub)<uint8_t>( a, b);
}

////////////////////////////////////////////////////////////////////////////////
// ( i * scale + add16 ) >> 8
#define SCALE_ADD8( i, scale, add16) ( (uint8_t)( (uint16_t)(         \
  (uint16_t)( (uint16_y)(uint8_t)(i) * (uint16_t)(uint8_t)(scale) )   \
                                     + (uint16_t)(add16)              \
  ) >> (uint16_t)8 ) )

//
// The above should compile to the correct MUL 8x8 instruction. The expected
// translation should have about 10 cycles, e.g.
//
//            ; A = _U8
//            EXG       A, XL           ; mov _U8 from A to XL
//            LD        A, _F8          ; load _F8 to A
//            MUL       X, A            ; X = XL * A
//            ADDW      X, _A16         ; X += _A16
//            LD        A, XH           ; return A = (X >> 8)
//
// See the following paper published by TI:
//
//      How to Write Multiplies Correctly in C Code:
//      http://www.ti.com/lit/an/spra683/spra683.pdf
//
// However, the IAR compiler converted this into a mul16 library call far too
// often. Upon further investigation, it seems that whenever the optimizer
// gets a hand on this, e.g. due to a constant in the multiplication,
// or even a variable used here being moved outside a loop, it ends up
// using the very expensive library call (44 cycles, plus parameter setup).
// Until this bug is fixed, it is not recommended to inline the macro or a
// C version without regular profiling.
//
//
// add16 adds to the raw result 16-bit result of the multiplication. It can be
// used for adding a value (<<8) or implementing rounding.
//
// The usual suspects for rounding are:
//
//  (0) add16 = 0x00, aka ROUND DOWN
//
//      * for inputs 1..255 and scale > 0, we get an output range of 0..254
//
//  (1) add16 = 0x80, aka ROUND
//
//      * this is the mathematically correct way to minimize rounding errors
//      * for inputs 1..255 and scale > 0, we get an output range of 0..254
//      * as a reminder, 255.0 * 255.0 / 256.0 == 254.00390625 !
//
//  (2) add16 = 0xFF, aka ROUND UP
//
//      * for inputs 1..255 and scale > 0, we get an output range of 1..255
//      * a scale of 255 leaves the input unchanged
//
//  (3) add16 = i
//
//       * for inputs 1..255 and scale > 0, we get an output range of 0..255
//       * a scale of 0 will result in 0 for all inputs
//       * a scale of 255 leaves the input unchanged
//
////////////////////////////////////////////////////////////////////////////////


#ifdef _STM8_PREFER_C_INLINE

// NOTE: The IAR 'optimizer' usually translates this into a very expensive
// mul16 library call, taking 50+ cycles instead of about 10. It's usually
// best to avoid unless you keep profiling / checking assembler output!
ALWAYS_INLINE CONST
inline uint8_t _STM8_F(scale_add8)( uint8_t i, fract8 scale, uint16_t add)
{
  return SCALE_ADD8( i, scale, add);
}

#else

_EXTERN_C

// NOTE: 21 cycles incl. calling overhead! Use the 'C' inlined version if possible
//       or use extended assembly mode to 'properly' inline assembly code.
// NOTE: i is actually 8-bit, the 16-bit type is used to aid register allocation
CONST
extern uint8_t _STM8_F(scale_add8)( uint16_t i, fract8 scale, uint16_t add);

_END_EXTERN_C

#endif

#ifdef _STM8_PREFER_C_INLINE
#else
#endif

// this seems to work very well on automatic / stack variables
// but IAR fails completely with a __tiny volatile global
// NOTE: BIG ENDIAN !!
template< typename T>
ALWAYS_INLINE
inline void _STM8_F(inc32)(T & l)
{
  static_assert( 0 == __LITTLE_ENDIAN__ , "Wrong architecture!?" );
  uint8_t b = ++((uint8_t*)&l)[3];
  if(!b) {
    b = ++((_stm8_reg32&)l).b[2];
    if(!b) {
      b = ++((_stm8_reg32&)l).b[1];
      if(!b) {
        ++((_stm8_reg32&)l).b[0];
      }
    }
  }
}


// simple calculation of log2(x) in O(log(N))
// note that log2(0) is undefined
// see math.c for better/faster alternatives
template< typename UINT >
ALWAYS_INLINE
inline uint8_t _STM8_F(log2)(UINT arg)
{
  uint8_t i = 0;
  while (arg>>=1)
    ++i;
  return i;
}


#endif // __STM8HAL_MATH_H