#include "instructions1.h"
#include "gb_processor.h"
#include "gb_memory.h"

static uint8_t *const reg8_table[8] = {
    &gb_proc.registers.r8.b,
    &gb_proc.registers.r8.c,
    &gb_proc.registers.r8.d,
    &gb_proc.registers.r8.e,
    &gb_proc.registers.r8.h,
    &gb_proc.registers.r8.l,
    NULL,
    &gb_proc.registers.r8.a};

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

void OP_0b00xxx100(void)
{
    uint8_t r_index = (gb_proc.opcode >> 3) & 0x7u;
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t value = *reg;
    uint8_t result = (uint8_t)(value + 1);

    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N);

    if ((value & 0x0F) == 0x0F)
        GB_FLAG_SET(GB_FLAG_H);
    else
        GB_FLAG_CLEAR(GB_FLAG_H);

    *reg = result;
    gb_proc.cycles += 1;
}

void OP_0b00xxx101(void)
{
    uint8_t r_index = (gb_proc.opcode >> 3) & 0x7u;
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t value = *reg;
    uint8_t result = (uint8_t)(value - 1);

    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_SET(GB_FLAG_N);

    if ((value & 0x0F) == 0x00)
        GB_FLAG_SET(GB_FLAG_H);
    else
        GB_FLAG_CLEAR(GB_FLAG_H);

    *reg = result;
    gb_proc.cycles += 1;
}

void OP_0b00xxx110(void)
{
    uint8_t r_index = (gb_proc.opcode >> 3) & 0x7u;
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t imm = mem_read(++gb_proc.pc);
    *reg = imm;

    gb_proc.cycles += 2;
}

void OP_0b00000111(void)
{
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t bit7 = (a & 0x80u) ? 1u : 0u;
    uint8_t res = (uint8_t)((a << 1) | bit7);

    gb_proc.registers.r8.a = res;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);
    if (bit7)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 1;
}