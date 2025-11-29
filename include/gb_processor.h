#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>

typedef struct
{
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
    } registers;

    uint16_t pc;
    uint16_t sp;
    uint8_t ime; /* interrupt master enable */
    uint8_t halted;
    uint8_t stopped;
    uint64_t cycles;

} GB_PROC;

extern GB_PROC gb_proc;

#endif