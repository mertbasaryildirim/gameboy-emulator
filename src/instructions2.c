#include "gb_processor.h"
#include "gb_memory.h"
#include "instructions2.h"

static uint8_t *const reg8_table[8] = {
    &gb_proc.registers.r8.b,
    &gb_proc.registers.r8.c,
    &gb_proc.registers.r8.d,
    &gb_proc.registers.r8.e,
    &gb_proc.registers.r8.h,
    &gb_proc.registers.r8.l,
    NULL,
    &gb_proc.registers.r8.a};

static const uint8_t cb_bit_base_table[4] = {0u, 2u, 4u, 6u};

static inline uint8_t cb_reg_index(uint8_t opcode)
{
    return opcode & 0x07u;
}

static inline uint8_t cb_bit_index(uint8_t opcode)
{
    uint8_t folded_row = (opcode >> 4) & 0x03u;
    uint8_t half = (opcode >> 3) & 0x01u;
    return (uint8_t)(cb_bit_base_table[folded_row] + half);
}

void OP_CB_0b00000xxx(void)
{
    uint8_t r_index = gb_proc.opcode & 0x07u;
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t value = *reg;
    uint8_t bit7 = (value & 0x80u) ? 1u : 0u;
    uint8_t result = (uint8_t)((value << 1) | bit7);

    *reg = result;

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (bit7)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_CB_0b00000110(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t value = mem_read(addr);

    uint8_t bit7 = (value & 0x80u) ? 1u : 0u;
    uint8_t result = (uint8_t)((value << 1) | bit7);

    mem_write(addr, result);

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (bit7)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 4;
}

void OP_CB_0b00001xxx(void)
{
    uint8_t r_index = gb_proc.opcode & 0x07u;
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t value = *reg;
    uint8_t bit0 = value & 0x01u;
    uint8_t result = (uint8_t)((value >> 1) | (uint8_t)(bit0 << 7));

    *reg = result;

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (bit0)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_CB_0b00001110(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t value = mem_read(addr);

    uint8_t bit0 = value & 0x01u;
    uint8_t result = (uint8_t)((value >> 1) | (uint8_t)(bit0 << 7));

    mem_write(addr, result);

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (bit0)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 4;
}

void OP_CB_0b00010xxx(void)
{
    uint8_t r_index = gb_proc.opcode & 0x07u;
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t value = *reg;
    uint8_t old_c = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;
    uint8_t new_c = (value & 0x80u) ? 1u : 0u;
    uint8_t result = (uint8_t)((value << 1) | old_c);

    *reg = result;

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (new_c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_CB_0b00010110(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t value = mem_read(addr);

    uint8_t old_c = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;
    uint8_t new_c = (value & 0x80u) ? 1u : 0u;
    uint8_t result = (uint8_t)((value << 1) | old_c);

    mem_write(addr, result);

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (new_c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 4;
}

void OP_CB_0b00011xxx(void)
{
    uint8_t r_index = gb_proc.opcode & 0x07u;
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t value = *reg;
    uint8_t old_c = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;
    uint8_t new_c = value & 0x01u;
    uint8_t result = (uint8_t)((value >> 1) | (uint8_t)(old_c << 7));

    *reg = result;

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (new_c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_CB_0b00011110(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t value = mem_read(addr);

    uint8_t old_c = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;
    uint8_t new_c = value & 0x01u;
    uint8_t result = (uint8_t)((value >> 1) | (uint8_t)(old_c << 7));

    mem_write(addr, result);

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (new_c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 4;
}

void OP_CB_0b00100xxx(void)
{
    uint8_t r_index = gb_proc.opcode & 0x07u;
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t value = *reg;
    uint8_t new_c = (value & 0x80u) ? 1u : 0u;
    uint8_t result = (uint8_t)(value << 1);

    *reg = result;

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (new_c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_CB_0b00100110(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t value = mem_read(addr);

    uint8_t new_c = (value & 0x80u) ? 1u : 0u;
    uint8_t result = (uint8_t)(value << 1);

    mem_write(addr, result);

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (new_c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 4;
}

void OP_CB_0b00101xxx(void)
{
    uint8_t r_index = gb_proc.opcode & 0x07u;
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t value = *reg;
    uint8_t new_c = value & 0x01u;
    uint8_t result = (uint8_t)((value >> 1) | (value & 0x80u));

    *reg = result;

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (new_c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_CB_0b00101110(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t value = mem_read(addr);

    uint8_t new_c = value & 0x01u;
    uint8_t result = (uint8_t)((value >> 1) | (value & 0x80u));

    mem_write(addr, result);

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (new_c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 4;
}

void OP_CB_0b00110xxx(void)
{
    uint8_t r_index = gb_proc.opcode & 0x07u;
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t value = *reg;
    uint8_t result = (uint8_t)((value >> 4) | (value << 4));

    *reg = result;

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_CB_0b00110110(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t value = mem_read(addr);

    uint8_t result = (uint8_t)((value >> 4) | (value << 4));

    mem_write(addr, result);

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    gb_proc.cycles += 4;
}

void OP_CB_0b00111xxx(void)
{
    uint8_t r_index = gb_proc.opcode & 0x07u;
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t value = *reg;
    uint8_t new_c = value & 0x01u;
    uint8_t result = (uint8_t)(value >> 1);

    *reg = result;

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (new_c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_CB_0b00111110(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t value = mem_read(addr);

    uint8_t new_c = value & 0x01u;
    uint8_t result = (uint8_t)(value >> 1);

    mem_write(addr, result);

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (new_c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 4;
}

void OP_CB_0b01xxxyyy(void)
{
    uint8_t r_index = cb_reg_index(gb_proc.opcode);
    uint8_t bit_index = cb_bit_index(gb_proc.opcode);
    uint8_t *reg = reg8_table[r_index];
    if (!reg)
        return;

    uint8_t value = *reg;
    uint8_t mask = (uint8_t)(1u << bit_index);

    if ((value & mask) == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N);
    GB_FLAG_SET(GB_FLAG_H);

    gb_proc.cycles += 2;
}