#ifndef __STM8HAL_VARIANTS_STM8S001J_H
#define __STM8HAL_VARIANTS_STM8S001J_H

#include "../pin.h"

#if !(MCU==STM8S001J3)
#warning "This variant (STM8S001J, Low Density, 8 pins) does not match MCU settings"
#endif

#ifndef STM8_VARIANT
#define STM8_VARIANT STM8Sx01J
#endif

////////////////////////////////////////////////////////////////////////////////
// STM8S001J3 (SO8N below)
// https://www.st.com/resource/en/datasheet/stm8s001j3.pdf
// These are supposed to have the same dies as the 003F3 variants, with
// multiple pins bonded to each pad.
//
//                       1|   H|8   PD5 (D10)(A5) TX
// RX!   (A6) (D11) PD6  1|H  H|8   PD3 (D9) (A4) PWM AETR
//            (D0)  PA1  1|!  !|8   PD1 (D8) SWIM
//                       1|   H|8   PC6 (D7) MOSI [PWM]
//                        |    |
//                  GND  2|   H|7   PC5 (D6)  SCK [PWM]
//                 Vss   2|   H|7   PC4 (D5)  PWM CCO [A2]
//                 Vssa  2|   H|7   PC3 (D4)  PWM [TLI]
//                        |    |
//                 Vcap  3|   T|6   PB4 (D3)  SCL [AETR]
//                        |    |
//                 Vdd   4|   T|5   PB5 (D2)  SDA
//                 Vdda  4|   H|5   PA3 (D1)  PWM [NSS] [TX!]
//                        +----+
//
// H: High Sink, High Speed; ¬: Low Speed; T: Open Drain; !: RTFM
//
// NOTE: As several pins provide a connection to multiple GPIOs,
// the mode selection for any of those GPIOs impacts all the other
// GPIOs connected to the same pin. The user is responsible for
// the proper setting of the GPIO modes in order to avoid conflicts
// between GPIOs bonded to the same pin (including their alternate
// functions). For example, pull-up enabled on PD1 is also seen on
// PC6, PD3 and PD5. Push-pull configuration of PC3 is also seen
// on PC4 and PC5, etc.
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
// NOTE: The PA2, PB0, PB1, PB2, PB3, PB6, PB7, PC1, PC2, PC7,
// PD0, PD2, PD4, PD7, PE5 and PF4 GPIOs should be configured
// after device reset in output push-pull mode with output
// low-state to reduce the device’s consumption and to improve
// its EMC immunity. The GPIOs mentioned above are not connected
// to pins, and they are in input-floating mode after a device
// reset.
}

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

#endif // __STM8HAL_VARIANTS_STM8S001J_H
