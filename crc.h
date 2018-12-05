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

#ifndef __STM8HAL_CRC_H
#define __STM8HAL_CRC_H

_EXTERN_C

// checksum (to be located at address 0x8000)
// extern uint16_t const __checksum @ 0x8000;


// CRC16 with a polygon of 0x1021 / 0x11021;
//   XMODEM:      init 0x0000; end value 'as is'; crc16("123456789") == 0x31C3
//   FALSE-CCITT: init 0xffff; end value 'as is'; crc16("123456789") == 0x29B1
// 49 bytes, 22 cycles/byte; 0.7MB/s @ 16MHz; req 2 bytes stack
extern uint16_t _STM8_F(crc16)(
  void const *addr,
  size_t num,
  uint16_t crc = 0x0000);

_END_EXTERN_C

////////////////////////////////////////////////////////////////////////////////
//
//  TESTING
//

#ifdef _STM8_TESTS
#ifdef __cplusplus
bool _stm8_tests_crc();
#endif
#endif

#endif // __STM8HAL_CRC_H