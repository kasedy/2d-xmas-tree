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

#pragma message(VAR_NAME_VALUE(COMPILED_ANIMATION))
