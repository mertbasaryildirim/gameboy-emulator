#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>

typedef struct
{
    union
    {
        struct
        {
            uint8_t f; /* AF low  */
            uint8_t a; /* AF high */

            uint8_t c; /* BC low  */
            uint8_t b; /* BC high */

            uint8_t e; /* DE low  */
            uint8_t d; /* DE high */

            uint8_t l; /* HL low  */
            uint8_t h; /* HL high */

            uint8_t spl; /* SP low  */
            uint8_t sph; /* SP high */
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
    uint8_t ime; /* interrupt master enable */
    uint8_t halted;
    uint8_t stopped;
    uint64_t cycles;

} GB_PROC;

extern GB_PROC gb_proc;

#endif