#pragma once

#define COMPILE_TIME_SIZEOF(t)      template<int s> struct SIZEOF_ ## t ## _IS; \
                                    struct foo { \
                                        int a,b; \
                                    }; \
                                    SIZEOF_ ## t ## _IS<sizeof(t)> SIZEOF_ ## t ## _IS;


/* definition to expand macro then apply to pragma message */
#define VALUE_TO_STRING(x...) #x
#define VALUE(x...) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var "="  VALUE(var)

// #pragma message(VAR_NAME_VALUE(COMPILED_ANIMATION))

#if F_CPU == 500000L // X16
  #define CPU_PRESCALER_BITS (1 << CLKPS2)
#elif F_CPU == 250000L // X32
  #define CPU_PRESCALER_BITS (1 << CLKPS2) | (1 << CLKPS0)
#elif F_CPU == 125000L // X64
  #define CPU_PRESCALER_BITS (1 << CLKPS2) | (1 << CLKPS1)
#elif F_CPU == 62500 // X128
  #define CPU_PRESCALER_BITS (1 << CLKPS2) | (1 << CLKPS1) | (1 << CLKPS0)
#elif F_CPU == 31250 // X256
  #define CPU_PRESCALER_BITS (1 << CLKPS3)
#endif

#ifdef CPU_PRESCALER_BITS
  #define SET_CPU_PRESCALER() \
    cli(); \
    CLKPR = (1 << CLKPCE); /* Clock prescaler change enable*/ \
    CLKPR = (CPU_PRESCALER_BITS); \
    sei();
#else
  #define SET_CPU_PRESCALER()
#endif

#define ARRAY_LEN(x) (sizeof(x) / sizeof(*x))