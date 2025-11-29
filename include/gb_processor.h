#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define GB_HOST_LITTLE_ENDIAN 1
#elif defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#define GB_HOST_BIG_ENDIAN 1
#elif defined(_WIN32) || defined(_WIN64)
#define GB_HOST_LITTLE_ENDIAN 1
#else
#error "Unknown host byte order: please define GB_HOST_LITTLE_ENDIAN or GB_HOST_BIG_ENDIAN."
#endif

typedef struct
{
    union
    {
#if defined(GB_HOST_LITTLE_ENDIAN)

        struct
        {
            uint8_t f;
            uint8_t a;

            uint8_t c;
            uint8_t b;

            uint8_t e;
            uint8_t d;

            uint8_t l;
            uint8_t h;

            uint8_t spl;
            uint8_t sph;
        } r8;

#elif defined(GB_HOST_BIG_ENDIAN)

        struct
        {
            uint8_t a;
            uint8_t f;

            uint8_t b;
            uint8_t c;

            uint8_t d;
            uint8_t e;

            uint8_t h;
            uint8_t l;

            uint8_t sph;
            uint8_t spl;
        } r8;

#endif

        struct
        {
            uint16_t af;
            uint16_t bc;
            uint16_t de;
            uint16_t hl;
            uint16_t sp;
        } r16;
    } registers;

    uint8_t opcode;
    uint16_t pc;
    uint8_t ime;
    uint8_t halted;
    uint8_t stopped;
    uint64_t cycles;

} GB_PROC;

extern GB_PROC gb_proc;

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
_Static_assert(sizeof(((GB_PROC *)0)->registers.r8) ==
                   sizeof(((GB_PROC *)0)->registers.r16),
               "r8 and r16 views must have identical size");
#endif

#endif
