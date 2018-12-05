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

#ifdef _STM8_TESTS
#include <stdio.h>
#endif

#if 0

  //  The polynomial is x^12 + x^5 + x^0, a.k.a. $1021 (00010000 00100001)
  //             [CRC_HI] [CRC_LO]
  //             DATABYTE
  //           = abcdefgh ijklmnop
  //                     ¦
  //           +         ?
  //           ¦ ijklmnop abcdefgh = Input, shifted left 8 bits (wraps around)
  //       XOR + ...abcde fgh..... = x^5
  //  Together + efgh.... ....abcd = x^12
  //           ¦ abcd...a bcd..... = Feedback
  //           +     ¦       ¦
  //                 ?       ?
  //             [CRC_HI] [CRC_LO]

  //   Or like it is implemented here, with qrst = abcd ^ efgh
  //
  //             [CRC_HI] [CRC_LO]
  //             DATABYTE
  //           = abcdefgh ijklmnop
  //                     ¦
  //           ^     abcd
  //           = abcdqrst
  //    SWAP   ¦ ijklmnop abcdqrst = Input, shifted left 8 bits (wraps around)
  //       XOR ^ ...abcdq rst..... = x^5
  //  Together ^ qrst.... ....~~~~ = x^12
  //           ^ ~~~~...~ ~~~..... = Feedback
  //           =     ¦       ¦
  //                 ?       ?
  //             [CRC_HI] [CRC_LO]

#endif


_EXTERN_C

#pragma optimize=speed
NO_INLINE
uint16_t _STM8_F(crc16)(void const *addr, size_t num, uint16_t crc)
{
  // X: addr; Y: num; ?b0 crc_hi; ?b1: crc_lo

  if(!num) goto exit;                     // idiot check

  asm("PUSH  A                  \n");     // backup A and
  asm("PUSH  A                  \n");     // create temp space
  asm("LD    A, s:?b0           \n");     // preload A with CRC hi byte

  for( ; num>0; --num)  // Y  - if used here, we don't need to preserve it
  {
    // EXPECTS: CRC HI byte in A; X addr of new data byte
    // NOTE: CRC lo and hi bytes are swapped during each iteration !!

    // 18 cycles CRC calculations, 4 cycles loop

    // crc_hi ^= databyte
    asm("XOR   A, (X)               \n"     // XOR data byte into CRC HI
        "MOV   s:?b0, s:?b1         \n"     // ?b0 is now CRC LO
        "LD    s:?b1, A             \n");   // ?b1 is now CRC HI
    // crc_hi ^= crc_hi >> 4
    asm("SWAP  A                    \n"     // A>>4
        "AND   A, #$0F              \n"     //        [    1234]
        "LD    (1,SP), A            \n"     // TMP1 = [    1234]
        "XOR   A, s:?b1             \n"     // XOR into CRC HI byte
        "LD    s:?b1, A             \n");   // [12345678]
    // tmp = [5678    ]
    asm("SWAP  A                    \n"     // [56781234]
        "AND   A, #$F0              \n"     // [5678    ]
        "PUSH  A                    \n");   // TMP2 = [5678    ]
    // crc_hi ^= [678     ]
    asm("SLA   A                    \n"     // [678     ], CARRY=5 ==|
        "XOR   A, s:?b1             \n"     // XOR into CRC HI byte  |
        "LD    s:?b1, A             \n");   // CRC HI DONE.          |
    // crc_lo ^ [5678    ] ^ [   12345]                              |
    asm("POP   A                    \n"     // TMP2 = [5678    ]     |
        "RLC   (1,SP)               \n"     // TMP1 = [   12345] <===|
        "XOR   A, (1,SP)            \n"     // [5678    ] ^ [   12345]
        "XOR   A, s:?b0             \n");   // and XOR everything into CRC LO

    // A is now the new CRC HI byte, CRC LO byte already in ?b1 !!

    asm("INCW  X                    \n");   //
  }

  asm("LD    s:?b0, A             \n");   // end loop, store A
  asm("POP   A                    \n");   // remove temp space
  asm("POP   A                    \n");   // restore A

exit:
  return crc;
}

_END_EXTERN_C

////////////////////////////////////////////////////////////////////////////////
//
// TESTING
//

#ifdef _STM8_TESTS
#ifdef __cplusplus

// CRC16 with a polygon of 0x1021 / 0x11021;
//   XMODEM:      init 0x0000; end value 'as is'; crc16("123456789") == 0x31C3
//   FALSE-CCITT: init 0xffff; end value 'as is'; crc16("123456789") == 0x29B1
const char text[] = "123456789"; // 0x0000 => 0x31C3, 0xFFFF => 0x29B1

NO_INLINE
OPTIMIZE_SPEED
bool _stm8_tests_crc()
{

  // XMODEM
  {
    uint16_t crc16 = 0x0000;
    if( 0x31C3 != _STM8_F(crc16)(text, sizeof(text)-1, crc16) )
    {
      puts("STM/Tests/CRC: XMODEM Test failed.");
      return false;
    }
    putchar('\n');
  }

  {
    uint16_t crc16 = 0x0000;
    for(uint8_t i=0; i< sizeof(text)-1; i++ )
    {
      crc16 = _STM8_F(crc16)(text+i, 1, crc16);
    }
    if( 0x31C3 != crc16 )
    {
      puts("STM/Tests/CRC: Bytewise test failed.");
      return false;
    }
    putchar('\n');
  }

  // FALSE-CCITT
  {
    uint16_t crc16 = 0xffff;
    if( 0x29B1 != _STM8_F(crc16)(text, sizeof(text)-1, crc16) )
    {
      puts("STM/Tests/CRC: FALSE-CCITT Test failed.");
      return false;
    }
    putchar('\n');
  }

  return true;
}

#endif // __cplusplus
#endif // #ifdef _STM8_TESTS


