#ifndef INSTRUCTION_TABLE2_H
#define INSTRUCTION_TABLE2_H

#include "stdint.h"

typedef void (*instruction2_handler_t)(void);

extern const instruction2_handler_t instruction_table2[256];

static inline instruction2_handler_t instruction2_get_handler(uint8_t opcode)
{
    return instruction_table2[opcode & 0xFFu];
}

#endif
