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

/// Various macros to help porting software between different compilers
/// (e.g. IAR, Raisonance, Cosmis, SDCC)

#ifndef __STM8HAL_COMPAT_H
#define __STM8HAL_COMPAT_H

#if !defined(__ICCSTM8__)
//included through stddef.h -> yvals.h
//
#define _GLUE_B(x,y) x##y
#define _GLUE(x,y) _GLUE_B(x,y)
//
#define _GLUE3_B(x,y,z) x##y##z
#define _GLUE3(x,y,z) _GLUE3_B(x,y,z)
//
#define _STRINGIFY_B(x) #x
#define _STRINGIFY(x) _STRINGIFY_B(x)
//
#endif

////////////////////////////////////////////////////////////////////////////////
//
// INTRINSICS (may have already been defined by STM's system headers)
//
#if !defined(__STM8A_H) && !defined(__STM8F_H) && !defined(__STM8S_H) && \
    !defined(__STM8L10x_H) && !defined(__STM8L15x_H)

#if defined(__RCST7__) // _RAISONANCE_

 #include <intrins.h>
 #define enableInterrupts()    _rim_()  /* enable interrupts */
 #define disableInterrupts()   _sim_()  /* disable interrupts */
 #define rim()                 _rim_()  /* enable interrupts */
 #define sim()                 _sim_()  /* disable interrupts */
 #define nop()                 _nop_()  /* No Operation */
 #define trap()                _trap_() /* Trap (soft IT) */
 #define wfi()                 _wfi_()  /* Wait For Interrupt */
 #define halt()                _halt_() /* Halt */

#elif defined(__CSMC__) // _COSMIC_

 #define enableInterrupts()    {_asm("rim\n");}  /* enable interrupts */
 #define disableInterrupts()   {_asm("sim\n");}  /* disable interrupts */
 #define rim()                 {_asm("rim\n");}  /* enable interrupts */
 #define sim()                 {_asm("sim\n");}  /* disable interrupts */
 #define nop()                 {_asm("nop\n");}  /* No Operation */
 #define trap()                {_asm("trap\n");} /* Trap (soft IT) */
 #define wfi()                 {_asm("wfi\n");}  /* Wait For Interrupt */
 #define halt()                {_asm("halt\n");} /* Halt */

#elif defined(__SDCC) // _SDCC_

 #define enableInterrupts()    {__asm__("rim\n");}  /* enable interrupts */
 #define disableInterrupts()   {__asm__("sim\n");}  /* disable interrupts */
 #define rim()                 {__asm__("rim\n");}  /* enable interrupts */
 #define sim()                 {__asm__("sim\n");}  /* disable interrupts */
 #define nop()                 {__asm__("nop\n");}  /* No Operation */
 #define trap()                {__asm__("trap\n");} /* Trap (soft IT) */
 #define wfi()                 {__asm__("wfi\n");}  /* Wait For Interrupt */
 #define halt()                {__asm__("halt\n");} /* Halt */

#elif defined(__ICCSTM8__) // _IAR_

 #include <intrinsics.h>
 #define enableInterrupts()    __enable_interrupt()   /* enable interrupts */
 #define disableInterrupts()   __disable_interrupt()  /* disable interrupts */
 #define rim()                 __enable_interrupt()   /* enable interrupts */
 #define sim()                 __disable_interrupt()  /* disable interrupts */
 #define nop()                 __no_operation()       /* No Operation */
 #define trap()                __trap()               /* Trap (soft IT) */
 #define wfi()                 __wait_for_interrupt() /* Wait For Interrupt */
 #define halt()                __halt()               /* Halt */

#endif // compiler

#endif // guards
//
////////////////////////////////////////////////////////////////////////////////
//
//

#if defined(__ICCSTM8__) // _IAR_

  #define bcpl(reg,_bit)  __BCPL((unsigned char *)&(reg),(_bit))
  #define bres(reg,_bit)  __BRES((unsigned char *)&(reg),(_bit))
  #define bset(reg,_bit)  __BSET((unsigned char *)&(reg),(_bit))

  #define MESSAGE(msg)    _Pragma( _STRINGIFY(message (msg)) )
  //#define WARNING(msg)    _Pragma( _STRINGIFY(warning (msg)) )
  #define ERROR(msg)      _Pragma( _STRINGIFY( error msg ) )

  #define TINY            __tiny        // __attribute__((tiny))
  #define NEAR            __near        // __attribute__((near))
  #define FAR             __far         // __attribute__((far))
  #define EEPROM          __eeprom      //
  #define IO(addr)        @ addr        // or pragma location // __attribute__((io(addr)))
  #define NO_INIT         __no_init
  // __attribute__((section(name)))
  #define SECTION(name)

  #define LOOP_IS_VOLATILE()     __asm("volatile_loop:\n")
  #define PREVENT_OPTIMIZATION() __asm("volatile:\n")

  #define OPTIMIZE_SIZE   _Pragma("optimize=size")
  #define OPTIMIZE_SPEED  _Pragma("optimize=speed")

  // __attribute__((optnone))
  #define NO_OPTIMIZE     _Pragma("optimize=none")

  // __attribute__((always_inline))
  #define ALWAYS_INLINE   _Pragma("inline=forced")

  // __attribute__((noinline))
  #define NO_INLINE       _Pragma("inline=never")

  // artificial??
  // #pragma inline=no_body

  // try to inline every call inside this function
  // __attribute__((flatten))
  #define FLATTEN

  // the function is a hot spot and optimized more aggressively
  // __attribute__((hot))
  #define HOT
  // __attribute__((cold))
  #define COLD

  // compiler should optimize for the case where a path of
  // execution through a statement is more or less likely
  // __attribute__((likely))
  #define LIKELY
  // __attribute__((unlikely))
  #define UNLIKELY

  // fall through from the previous case label is intentional and should
  // not be diagnosed by a compiler that warns on fall-through
  #define FALLTHROUGH     __attribute__((fallthrough))

  // indicates that the use of the name or entity declared with this attribute
  // is allowed, but discouraged for some reason
  #define DEPRECATED(msg)  __attribute__((deprecated(msg)))

  // indicates that the function does not return
  // __attribute__((noreturn))
  #define NO_RETURN       __noreturn

  // indicates that a function cannot throw an exception
  #define NO_THROW        __attribute__((nothrow))

  // functions has no side effects and depends only on parameters and global variables
  // __attribute__((pure))
  #define PURE            _Pragma("function_effects=no_state, always_returns")

  // like PURE, but function is not allowed to read global memory
  // Note that a function that has pointer arguments and examines the data
  // pointed to must not be declared const
  //__attribute__((const))
  #define CONST           _Pragma("function_effects=no_state, always_returns")

  // encourages the compiler to issue a warning if the return value is discarded
  // __attribute__((warn_unused_result))
  #define NO_DISCARD

  // called before execution enters main
  // __attribute__((constructor))
  #define CONSTRUCTOR     _Pragma(" error\"Consider implementing __low_level_init\"" )

  // suppresses compiler warnings on unused entities, if any
  // [[maybe_unused]](C++17)
  #define UNUSED        __attribute__((unused))

  // code must be emitted even if the function or variable is unreferenced
  // __attribute__((used))
  #define USED            __root

  // indicate that the function has a hidden requirement for symbol (or: KEEP)
  #define REQUIRED(sym)   _Pragma( _STRINGIFY( required=sym ))

  // function can be overridden by other code, see also: pragma weak
  // __attribute__((weak))
  #define WEAK            __weak

  // indicate that the specified function is an interrupt handler
  // __attribute__((interrupt( irq )))
  #define INTERRUPT(irq)  _Pragma( _STRINGIFY( vector = irq )) \
  __interrupt

  // disable interrupts for the duration of the given function
  // __attribute__((disinterrupt))
  #define NO_INTERRUPTS   __monitor

  // function does not need prologue/epilogue sequences
  // __attribute__((naked))
  #define NAKED

  #define __TASK          __task

  #define EXPECTS( expr )
  #define ENSURES( expr )
  //#define ASSERT( expr )

  // specifies the smallest possible alignment; not supported on structs?
  // __attribute__((packed))
  #define PACKED


#if defined(__cplusplus)

  class Mutex
  {
    __istate_t mState;
  public:
    Mutex() : mState( __get_interrupt_state() ) { __disable_interrupt(); }
   ~Mutex() { __set_interrupt_state(mState); }
  };

#endif

# ifndef NDEBUG
  #define debug_break()   __asm("DC8 $8B") // TODO: call __DebugBreak() ??
# endif

#elif defined(__RCST7__) // _RAISONANCE_

 #define TINY             page0
 #define NEAR             data
 #define FAR              far
 #define CONST            code

#elif defined(__CSMC__) // _COSMIC_

  #define TINY            @tiny
  #define NEAR            @near
  #define FAR             @far
  #define CONST           const

#elif defined(__SDCC) // _SDCC_

#endif // compiler

// https://github.com/scottt/debugbreak
#ifdef NDEBUG
  #define debug_break() do {} while(0)
#endif

#define MAIN_ATTRIBUTES NO_RETURN

#define BEGIN_CRITICAL  {
#define END_CRITICAL    }
#define FORCE_REFERENCE(var) do { (void)var; } while(false);

#if __cplusplus <= 199711L
#define STATIC_ASSERT(expression, message)
#define constexpr /*const*/
#else
#define STATIC_ASSERT(expression, message) static_assert(expression, message)
#endif // __cplusplus <= 199711L

#endif // __STM8HAL_COMPAT_H//