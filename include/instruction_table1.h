#ifndef INSTRUCTION_TABLE1_H
#define INSTRUCTION_TABLE1_H

#include "stdint.h"

typedef void (*instruction1_handler_t)(void);

extern const instruction1_handler_t instruction_table1[256];

static inline instruction1_handler_t instruction1_get_handler(uint8_t opcode)
{
    return instruction_table1[opcode & 0xFFu];
}

#endif