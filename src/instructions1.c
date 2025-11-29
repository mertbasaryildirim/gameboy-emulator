#include "instructions1.h"
#include "gb_processor.h"
#include "gb_memory.h"

void OP_0b00000000(void)
{
    gb_proc.cycles += 1;
}