#ifndef __STM8HAL_VARIANTS_STM8Sx03F_H
#define __STM8HAL_VARIANTS_STM8Sx03F_H

#include "../pin.h"

#if !(MCU==STM8S103F2) && !(MCU==STM8S103F3) && !(MCU==STM8S003F3)
#warning "This variant (STM8Sx03F, Low Density, 20 pins) does not match MCU settings"
#endif

#ifndef STM8_VARIANT
#define STM8_VARIANT STM8Sx03F
#endif

////////////////////////////////////////////////////////////////////////////////
// STM8S103F2/3 (TSSOP20/SO20, below, UFQFPN20 with same logical pinout)
// https://www.st.com/resource/en/datasheet/stm8s103f2.pdf
// STM8S003F3
// https://www.st.com/resource/en/datasheet/stm8s003f3.pdf
//
//                        +---u---+
// PWM [BEEP] (D13) PD4  1|H     H|20  PD3 (D12) (A4) PWM AETR
// TX    (A5) (D14) PD5  2|H     H|19  PD2 (D11) (A3) [PWM]
// RX    (A6) (D15) PD6  3|H     H|18  PD1 (D10)
//                 NRST  4|      H|17  PC7 (D9)  MISO [PWM]
//            (D0)  PA1  5|¬     H|16  PC6 (D8)  MOSI [PWM]
//            (D1)  PA2  6|¬     H|15  PC5 (D7)   SCK [PWM]
//              GND/Vss  7|      H|14  PC4 (D6)  (A2) PWM CCO
//                 Vcap  8|      H|13  PC3 (D5)  PWM [TLI]
//                  Vdd  9|      T|12  PB4 (D4)  SCL [AETR]
// PWM  [NSS] (D2)  PA3 10|H     T|11  PB5 (D3)  SDA <LED>
//                        +-------+
//
// H: High Sink, High Speed; ¬: Low Speed; T: True Open Drain; !: RTFM
//
//
// NOTE: Unused I/O pins must not be left floating and put into one of the
// following configurations to avoid extra current consumption and EMC issues:
//
//  (a) input floating (reset state) with external pull-up/down resistors;
//  (b) input floating (reset state) with internal pull-up/down resistors;
//  (c) configured as output push-pull low, providing the best EMC immunity.
//
// Unless otherwise specified in the datasheet, the I/O ports not present
// on smaller packages are automatically configured by a factory setting.
// Otherwise, they need to be configured into push-pull low here.
//
inline void _stm8_config_unbonded_pins()
{
}
//

#define NUM_DIGITAL_PINS            16
#define MAX_PIN                     15

enum STM8PortPin {
	PA1 = 0, PA2, PA3,
	PB5 = 3, PB4,
	PC3 = 5, PC4, PC5, PC6, PC7,
	PD1 = 10, PD2, PD3, PD4, PD5, PD6 = MAX_PIN
};

_STM8_DEF_PIN(A,1); _STM8_DEF_PIN(A,2); _STM8_DEF_PIN(A,3);
_STM8_DEF_PIN(B,5); _STM8_DEF_PIN(B,4);
_STM8_DEF_PIN(C,3); _STM8_DEF_PIN(C,4); _STM8_DEF_PIN(C,5); _STM8_DEF_PIN(C,6); _STM8_DEF_PIN(C,7);
_STM8_DEF_PIN(D,1); _STM8_DEF_PIN(D,2); _STM8_DEF_PIN(D,3); _STM8_DEF_PIN(D,4); _STM8_DEF_PIN(D,5); _STM8_DEF_PIN(D,6);

#define PIN_SPI_SS          (PA3)
#define PIN_SPI_MOSI        (PC6)
#define PIN_SPI_MISO        (PC7)
#define PIN_SPI_SCK         (PC5)
#define PIN_WIRE_SDA        (PB5)
#define PIN_WIRE_SCL        (PB4)
#define PIN_TX	            (PD5)
#define PIN_RX	            (PD6)

#define PIN_LED_BUILTIN     (PB5)

#define SPI_DATA       PIN_SPI_MOSI
#define SPI_CLOCK      PIN_SPI_SCK


#endif // __STM8HAL_VARIANTS_STM8Sx03F_H
