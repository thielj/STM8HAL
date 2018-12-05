#ifndef __STM8HAL_VARIANTS_STM8Sx03K_H
#define __STM8HAL_VARIANTS_STM8Sx03K_H

#include "../pin.h"

#if !(MCU==STM8S103K3) && !(MCU==STM8S003K3)
#warning "This variant (STM8Sx03K, Low Density, 32 pins) does not match MCU settings"
#endif

#ifndef STM8_VARIANT
#define STM8_VARIANT STM8Sx03K
#endif

////////////////////////////////////////////////////////////////////////////////
// STM8S103K3 (SDIP32, below, LQFP32, UFQFPN32 with same logical pinout)
// https://www.st.com/resource/en/datasheet/stm8s103f2.pdf
// STM8S003K3 (Value Line)
// https://www.st.com/resource/en/datasheet/stm8s003f3.pdf
//
//                        +---u---+
// [AETR] PWM (D23) PD3  1|H     H|32 PD2 (D22)      [PWM]
// [BEEP] PWM (D24) PD4  2|H     !|31 PD1 (D21) SWIM
//        TX  (D25) PD5  3|H     H|30 PD0 (D20)  PWM [CCO]
//        RX  (D26) PD6  4|H     H|29 PC7 (D19) MISO
// [PWM]  TLI (D27) PD7  5|H     H|28 PC6 (D18) MOSI
//                 NRST  6|      H|27 PC5 (D17)  SCK
//            (D0)  PA1  7|¬     H|26 PC4 (D16)  PWM  CCO
//            (D1)  PA2  8|¬     H|25 PC3 (D15)  PWM
//              GND/Vss  9|      H|24 PC2 (D14)  PWM
//                 Vcap 10|      H|23 PC1 (D13)  PWM
//                  Vdd 11|      H|22 PE5 (D12)  NSS
// [NSS]  PWM (D2)  PA3 12|H     H|21 PB0 (D11) (A0) {PWM}
//            (D3)  PF4 13|!     H|20 PB1 (D10) (A1) {PWM}
//            (D4)  PB7 14|¬     H|19 PB2 (D9)  (A2) {PWM}
//            (D5)  PB6 15|      H|18 PB3 (D8)  (A3) [ETR]
//        SDA (D6)  PB5 16|T     T|17 PB4 (D7)   SCL
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

#define NUM_DIGITAL_PINS            28
#define MAX_PIN                     27

enum STM8PortPin {
	PA1 = 0, PA2, PA3,
        PF4 = 3,
        PB7 = 4, PB6, PB5, PB4, PB3, PB2, PB1, PB0,
        PE5 = 12,
        PC1 = 13, PC2, PC3, PC4, PC5, PC6, PC7,
        PD0 = 20, PD1, PD2, PD3, PD4, PD5, PD6, PD7 = MAX_PIN
};

_STM8_DEF_PIN(A,1); _STM8_DEF_PIN(A,2); _STM8_DEF_PIN(A,3);
_STM8_DEF_PIN(F,4);
_STM8_DEF_PIN(B,7); _STM8_DEF_PIN(B,6); _STM8_DEF_PIN(B,5); _STM8_DEF_PIN(B,4); _STM8_DEF_PIN(B,3); _STM8_DEF_PIN(B,2); _STM8_DEF_PIN(B,1); _STM8_DEF_PIN(B,0);
_STM8_DEF_PIN(E,5);
_STM8_DEF_PIN(C,1); _STM8_DEF_PIN(C,2); _STM8_DEF_PIN(C,3); _STM8_DEF_PIN(C,4); _STM8_DEF_PIN(C,5); _STM8_DEF_PIN(C,6); _STM8_DEF_PIN(C,7);
_STM8_DEF_PIN(D,0); _STM8_DEF_PIN(D,1); _STM8_DEF_PIN(D,2); _STM8_DEF_PIN(D,3); _STM8_DEF_PIN(D,4); _STM8_DEF_PIN(D,5); _STM8_DEF_PIN(D,6); _STM8_DEF_PIN(D,7);

#define PIN_SPI_SS          (PA3)
#define PIN_SPI_MOSI        (PC6)
#define PIN_SPI_MISO        (PC7)
#define PIN_SPI_SCK         (PC5)
#define PIN_I2C_SDA         (PB5)
#define PIN_I2C_SCL         (PB4)
#define PIN_TX	            (PD5)
#define PIN_RX	            (PD6)

//#define PIN_LED_BUILTIN     (PB5) // TODO: verify

#define SPI_DATA       PIN_SPI_MOSI
#define SPI_CLOCK      PIN_SPI_SCK

#endif // __STM8HAL_VARIANTS_STM8Sx03K_H
