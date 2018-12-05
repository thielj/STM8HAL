#ifndef __STM8HAL_VARIANTS_STM8Sx05K_H
#define __STM8HAL_VARIANTS_STM8Sx05K_H

#include "../pin.h"

#if !(MCU==STM8S105K4) && !(MCU==STM8S105K6) && !(MCU==STM8S105K6) \
 && !(MCU==STM8S005K6)
#warning "This variant (STM8Sx05K, Medium Density, 32 pins) does not match MCU settings"
#endif

#ifndef STM8_VARIANT
#define STM8_VARIANT STM8Sx05K
#endif

////////////////////////////////////////////////////////////////////////////////
// STM8S105K4/6 (SDIP32, below, LQFP32, UFQFPN32 with same logical pinout)
// https://www.st.com/resource/en/datasheet/stm8s105k4.pdf
// STM8S005K6 (Value Line)
// https://www.st.com/resource/en/datasheet/stm8s005k6.pdf
//
//                        +---u---+
// [AETR] PWM (D20) PD3  1|H     H|32 PD2 (D19)  PWM [PWM]
// [BEEP] PWM (D21) PD4  2|H     !|31 PD1 (D18) SWIM
//        TX  (D22) PD5  3|¬     H|30 PD0 (D17)  PWM [CCO]
//        RX  (D23) PD6  4|¬     H|29 PC7 (D16) MISO
// [PWM]  TLI (D24) PD7  5|¬     H|28 PC6 (D15) MOSI
//                 NRST  6|      H|27 PC5 (D14)  SCK
//            (D0)  PA1  7|¬     H|26 PC4 (D13)  PWM
//            (D1)  PA2  8|¬     H|25 PC3 (D12)  PWM
//              GND/Vss  9|      H|24 PC2 (D11)  PWM
//                 Vcap 10|      H|23 PC1 (D10)  PWM
//                  Vdd 11|      ¬|22 PE5 (D9)   NSS
//                Vddio 12|      ¬|21 PB0 (D8)  (A0) [PWM]
//      (A12) (D2)  PF4 13|!     ¬|20 PB1 (D7)  (A1) [PWM]
//                 Vdda 14|      ¬|19 PB2 (D6)  (A2) [PWM]
//            AGND/Vssa 15|      ¬|18 PB3 (D5)  (A3) [ETR]
// [SDA] (A5) (D3)  PB5 16|¬     ¬|17 PB4 (D4)  (A4) [SCL]
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

#define NUM_DIGITAL_PINS            25
#define MAX_PIN                     24

enum STM8PortPin {
	PA1 = 0, PA2,
        PF4 = 2,
        PB5 = 3, PB4, PB3, PB2, PB1, PB0,
        PE5 = 9,
        PC1 = 10, PC2, PC3, PC4, PC5, PC6, PC7,
        PD0 = 17, PD1, PD2, PD3, PD4, PD5, PD6, PD7 = MAX_PIN
};

_STM8_DEF_PIN(A,1); _STM8_DEF_PIN(A,2);
_STM8_DEF_PIN(F,4);
_STM8_DEF_PIN(B,5); _STM8_DEF_PIN(B,4); _STM8_DEF_PIN(B,3); _STM8_DEF_PIN(B,2); _STM8_DEF_PIN(B,1); _STM8_DEF_PIN(B,0);
_STM8_DEF_PIN(E,5);
_STM8_DEF_PIN(C,1); _STM8_DEF_PIN(C,2); _STM8_DEF_PIN(C,3); _STM8_DEF_PIN(C,4); _STM8_DEF_PIN(C,5); _STM8_DEF_PIN(C,6); _STM8_DEF_PIN(C,7);
_STM8_DEF_PIN(D,0); _STM8_DEF_PIN(D,1); _STM8_DEF_PIN(D,2); _STM8_DEF_PIN(D,3); _STM8_DEF_PIN(D,4); _STM8_DEF_PIN(D,5); _STM8_DEF_PIN(D,6); _STM8_DEF_PIN(D,7);

#define PIN_SPI_SS          (PE5)
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

#endif // __STM8HAL_VARIANTS_STM8Sx05K_H
