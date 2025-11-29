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
#if GB_HOST_LITTLE_ENDIAN
        struct
        {
            uint8_t f, a; // AF low, high
            uint8_t c, b; // BC low, high
            uint8_t e, d; // DE low, high
            uint8_t l, h; // HL low, high
            uint8_t spl, sph;
        } r8;
#elif GB_HOST_BIG_ENDIAN
        struct
        {
            uint8_t a, f; // AF high, low
            uint8_t b, c; // BC high, low
            uint8_t d, e; // DE high, low
            uint8_t h, l; // HL high, low
            uint8_t sph, spl;
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
