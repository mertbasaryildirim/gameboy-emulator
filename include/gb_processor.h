#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdint.h>

typedef struct
{
    uint16_t pc;
} GB_PROC;

extern GB_PROC gb_proc;

#endif