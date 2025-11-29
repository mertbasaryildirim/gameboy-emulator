#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>

typedef struct
{
    union
    {
        struct
        {
            uint8_t f, a; // AF low, high
            uint8_t c, b; // BC low, high
            uint8_t e, d; // DE low, high
            uint8_t l, h; // HL low, high
            uint8_t spl, sph;
        } r8;
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

#endif
