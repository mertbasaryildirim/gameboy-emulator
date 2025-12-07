#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>

/* Flag bit masks (F register) */
#define GB_FLAG_Z 0x80u
#define GB_FLAG_N 0x40u
#define GB_FLAG_H 0x20u
#define GB_FLAG_C 0x10u

/* Flag helper macros */
#define GB_FLAG_SET(mask) (gb_proc.registers.r8.f |= (uint8_t)(mask))
#define GB_FLAG_CLEAR(mask) (gb_proc.registers.r8.f &= (uint8_t)~(mask))
#define GB_FLAG_IS_SET(mask) ((gb_proc.registers.r8.f & (uint8_t)(mask)) != 0)

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
    uint8_t ime_enable_pending;
    uint8_t ime;
    uint8_t halted;
    uint8_t stopped;
    uint64_t cycles;

} GB_PROC;

extern GB_PROC gb_proc;

#endif
