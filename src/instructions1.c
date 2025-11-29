#include "instructions1.h"
#include "gb_processor.h"
#include "gb_memory.h"

void OP_0b00000000(void)
{
    gb_proc.cycles += 1;
}

void OP_0b00xx0001(void)
{
    uint8_t xx = (gb_proc.opcode >> 4) & 0x3u;

    uint8_t nn_l = mem_read(++gb_proc.pc);
    uint8_t nn_h = mem_read(++gb_proc.pc);
    uint16_t nn = (uint16_t)nn_l | ((uint16_t)nn_h << 8);

    uint16_t *rr_base = &gb_proc.registers.r16.bc;
    rr_base[xx] = nn;

    gb_proc.cycles += 3;
}

void OP_0b00000010(void)
{
    uint16_t addr = gb_proc.registers.r16.bc;
    uint8_t data = gb_proc.registers.r8.a;

    mem_write(addr, data);

    gb_proc.cycles += 2;
}

void OP_0b00xx0011(void)
{
    uint8_t xx = (gb_proc.opcode >> 4) & 0x3u;

    uint16_t *rr_base = &gb_proc.registers.r16.bc;
    rr_base[xx] += 1;

    gb_proc.cycles += 2;
}