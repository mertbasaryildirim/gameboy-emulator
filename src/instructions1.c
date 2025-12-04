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

void OP_0b00001000(void)
{
    uint8_t nn_l = mem_read(++gb_proc.pc);
    uint8_t nn_h = mem_read(++gb_proc.pc);
    uint16_t nn = (uint16_t)nn_l | ((uint16_t)nn_h << 8);

    uint16_t sp = gb_proc.registers.r16.sp;

    mem_write(nn, (uint8_t)(sp & 0x00FF));
    mem_write(nn + 1, (uint8_t)(sp >> 8));

    gb_proc.cycles += 5;
}

void OP_0b00xx1001(void)
{
    uint8_t xx = (gb_proc.opcode >> 4) & 0x3u;

    uint16_t *rr_base = &gb_proc.registers.r16.bc;
    uint16_t rr = rr_base[xx];
    uint16_t hl = gb_proc.registers.r16.hl;

    uint32_t result32 = (uint32_t)hl + (uint32_t)rr;
    uint16_t result16 = (uint16_t)result32;

    GB_FLAG_CLEAR(GB_FLAG_N);

    if (((hl & 0x0FFFu) + (rr & 0x0FFFu)) & 0x1000u)
        GB_FLAG_SET(GB_FLAG_H);
    else
        GB_FLAG_CLEAR(GB_FLAG_H);

    if (result32 & 0x10000u)
        GB_FLAG_SET(GB_FLAG_C);
    else
        GB_FLAG_CLEAR(GB_FLAG_C);

    gb_proc.registers.r16.hl = result16;

    gb_proc.cycles += 2;
}

void OP_0b00001010(void)
{
    uint16_t addr = gb_proc.registers.r16.bc;
    gb_proc.registers.r8.a = mem_read(addr);

    gb_proc.cycles += 2;
}

void OP_0b00xx1011(void)
{
    uint8_t xx = (gb_proc.opcode >> 4) & 0x3u;

    uint16_t *rr_base = &gb_proc.registers.r16.bc;
    rr_base[xx] -= 1;

    gb_proc.cycles += 2;
}

void OP_0b00001111(void)
{
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t bit0 = (a & 0x01u) ? 1u : 0u;
    uint8_t res = (uint8_t)((a >> 1) | (uint8_t)(bit0 << 7));

    gb_proc.registers.r8.a = res;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);
    if (bit0)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 1;
}

void OP_0b00010000(void)
{
    /* STOP is 2 bytes: opcode + dummy/parameter.
       Fetch and ignore the second byte so PC skips it. */
    (void)mem_read(++gb_proc.pc);
    gb_proc.stopped = 1;
    gb_proc.cycles += 1;
}

void OP_0b00010010(void)
{
    uint16_t addr = gb_proc.registers.r16.de;
    uint8_t data = gb_proc.registers.r8.a;

    mem_write(addr, data);

    gb_proc.cycles += 2;
}

void OP_0b00010111(void)
{
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t old_c = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;
    uint8_t new_c = (a & 0x80u) ? 1u : 0u;
    uint8_t result = (uint8_t)((a << 1) | old_c);

    gb_proc.registers.r8.a = result;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);
    if (new_c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 1;
}

void OP_0b00011000(void)
{
    int8_t e = (int8_t)mem_read(++gb_proc.pc);
    gb_proc.pc = (uint16_t)(gb_proc.pc + (int16_t)e);

    gb_proc.cycles += 3;
}

void OP_0b00011010(void)
{
    uint16_t addr = gb_proc.registers.r16.de;
    gb_proc.registers.r8.a = mem_read(addr);

    gb_proc.cycles += 2;
}

void OP_0b00011111(void)
{
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t old_c = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;
    uint8_t bit0 = (a & 0x01u) ? 1u : 0u;

    uint8_t result = (uint8_t)((a >> 1) | (uint8_t)(old_c << 7));

    gb_proc.registers.r8.a = result;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);
    if (bit0)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 1;
}

void OP_0b001xx000(void)
{
    uint8_t cc = (gb_proc.opcode >> 3) & 0x3u;

    int8_t e = (int8_t)mem_read(++gb_proc.pc);
    uint8_t z = GB_FLAG_IS_SET(GB_FLAG_Z);
    uint8_t c = GB_FLAG_IS_SET(GB_FLAG_C);

    uint8_t take;
    switch (cc)
    {
    case 0: /* NZ */
        take = !z;
        break;
    case 1: /* Z */
        take = z;
        break;
    case 2: /* NC */
        take = !c;
        break;
    default: /* 3: C */
        take = c;
        break;
    }

    if (take)
    {
        gb_proc.pc = (uint16_t)(gb_proc.pc + (int16_t)e);
        gb_proc.cycles += 3;
    }
    else
    {
        gb_proc.cycles += 2;
    }
}

void OP_0b00100010(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t data = gb_proc.registers.r8.a;

    mem_write(addr, data);
    gb_proc.registers.r16.hl = (uint16_t)(addr + 1);

    gb_proc.cycles += 2;
}

void OP_0b00100111(void)
{
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t adjust = 0;
    uint8_t c = GB_FLAG_IS_SET(GB_FLAG_C);
    uint8_t n = GB_FLAG_IS_SET(GB_FLAG_N);
    uint8_t h = GB_FLAG_IS_SET(GB_FLAG_H);

    if (!n)
    {
        if (c || a > 0x99u)
        {
            adjust |= 0x60u;
            c = 1;
        }
        if (h || (a & 0x0Fu) > 0x09u)
            adjust |= 0x06u;

        a = (uint8_t)(a + adjust);
    }
    else
    {
        if (c)
            adjust |= 0x60u;
        if (h)
            adjust |= 0x06u;

        a = (uint8_t)(a - adjust);
    }

    gb_proc.registers.r8.a = a;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_H | GB_FLAG_C);
    if (a == 0)
        GB_FLAG_SET(GB_FLAG_Z);
    if (c)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 1;
}

void OP_0b00101010(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;

    gb_proc.registers.r8.a = mem_read(addr);
    gb_proc.registers.r16.hl = (uint16_t)(addr + 1);

    gb_proc.cycles += 2;
}

void OP_0b00101111(void)
{
    gb_proc.registers.r8.a = (uint8_t)~gb_proc.registers.r8.a;

    GB_FLAG_SET(GB_FLAG_N | GB_FLAG_H);

    gb_proc.cycles += 1;
}

void OP_0b00110010(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t data = gb_proc.registers.r8.a;

    mem_write(addr, data);
    gb_proc.registers.r16.hl = (uint16_t)(addr - 1);

    gb_proc.cycles += 2;
}

void OP_0b00110110(void)
{
    uint8_t n = mem_read(++gb_proc.pc);
    mem_write(gb_proc.registers.r16.hl, n);

    gb_proc.cycles += 3;
}

void OP_0b00110111(void)
{
    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H);
    GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 1;
}

void OP_0b00111010(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;

    gb_proc.registers.r8.a = mem_read(addr);
    gb_proc.registers.r16.hl = (uint16_t)(addr - 1);

    gb_proc.cycles += 2;
}

void OP_0b00111111(void)
{
    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H);

    if (GB_FLAG_IS_SET(GB_FLAG_C))
        GB_FLAG_CLEAR(GB_FLAG_C);
    else
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 1;
}

void OP_0b01xxxxyy(void)
{
    uint8_t dst_idx = (gb_proc.opcode >> 3) & 0x07u;
    uint8_t src_idx = gb_proc.opcode & 0x07u;

    uint8_t *dst = reg8_table[dst_idx];
    uint8_t *src = reg8_table[src_idx];

    *dst = *src;

    gb_proc.cycles += 1;
}

void OP_0b01xxx110(void)
{
    uint8_t dst_idx = (gb_proc.opcode >> 3) & 0x07u;

    *reg8_table[dst_idx] = mem_read(gb_proc.registers.r16.hl);

    gb_proc.cycles += 2;
}

void OP_0b01110xxx(void)
{
    uint8_t src_idx = gb_proc.opcode & 0x07u;
    uint8_t value = *reg8_table[src_idx];

    mem_write(gb_proc.registers.r16.hl, value);

    gb_proc.cycles += 2;
}

void OP_0b10000xxx(void)
{
    uint8_t src_idx = gb_proc.opcode & 0x07u;
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t value = *reg8_table[src_idx];

    uint16_t sum = (uint16_t)a + (uint16_t)value;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (((a & 0x0Fu) + (value & 0x0Fu)) > 0x0Fu)
        GB_FLAG_SET(GB_FLAG_H);
    if (sum & 0x100u)
        GB_FLAG_SET(GB_FLAG_C);

    a = (uint8_t)sum;
    gb_proc.registers.r8.a = a;

    if (a == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.cycles += 1;
}

void OP_0b10000110(void)
{
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t value = mem_read(gb_proc.registers.r16.hl);

    uint16_t sum = (uint16_t)a + (uint16_t)value;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (((a & 0x0Fu) + (value & 0x0Fu)) > 0x0Fu)
        GB_FLAG_SET(GB_FLAG_H);
    if (sum & 0x100u)
        GB_FLAG_SET(GB_FLAG_C);

    a = (uint8_t)sum;
    gb_proc.registers.r8.a = a;

    if (a == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.cycles += 2;
}

void OP_0b10001xxx(void)
{
    uint8_t src_idx = gb_proc.opcode & 0x07u;
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t value = *reg8_table[src_idx];
    uint8_t carry_in = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;

    uint16_t sum = (uint16_t)a + (uint16_t)value + (uint16_t)carry_in;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (((a & 0x0Fu) + (value & 0x0Fu) + carry_in) > 0x0Fu)
        GB_FLAG_SET(GB_FLAG_H);
    if (sum & 0x100u)
        GB_FLAG_SET(GB_FLAG_C);

    a = (uint8_t)sum;
    gb_proc.registers.r8.a = a;

    if (a == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.cycles += 1;
}

void OP_0b10001110(void)
{
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t value = mem_read(gb_proc.registers.r16.hl);
    uint8_t carry_in = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;

    uint16_t sum = (uint16_t)a + (uint16_t)value + (uint16_t)carry_in;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (((a & 0x0Fu) + (value & 0x0Fu) + carry_in) > 0x0Fu)
        GB_FLAG_SET(GB_FLAG_H);
    if (sum & 0x100u)
        GB_FLAG_SET(GB_FLAG_C);

    a = (uint8_t)sum;
    gb_proc.registers.r8.a = a;

    if (a == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.cycles += 2;
}

void OP_0b10010xxx(void)
{
    uint8_t src_idx = gb_proc.opcode & 0x07u;
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t value = *reg8_table[src_idx];

    uint8_t result = (uint8_t)(a - value);

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_H | GB_FLAG_C);
    GB_FLAG_SET(GB_FLAG_N);

    if ((a & 0x0Fu) < (value & 0x0Fu))
        GB_FLAG_SET(GB_FLAG_H);
    if (a < value)
        GB_FLAG_SET(GB_FLAG_C);
    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.registers.r8.a = result;
    gb_proc.cycles += 1;
}

void OP_0b10010110(void)
{
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t value = mem_read(gb_proc.registers.r16.hl);

    uint8_t result = (uint8_t)(a - value);

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_H | GB_FLAG_C);
    GB_FLAG_SET(GB_FLAG_N);

    if ((a & 0x0Fu) < (value & 0x0Fu))
        GB_FLAG_SET(GB_FLAG_H);
    if (a < value)
        GB_FLAG_SET(GB_FLAG_C);
    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.registers.r8.a = result;
    gb_proc.cycles += 2;
}

void OP_0b10011xxx(void)
{
    uint8_t src_idx = gb_proc.opcode & 0x07u;
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t value = *reg8_table[src_idx];
    uint8_t carry_in = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;

    uint8_t result = (uint8_t)(a - value - carry_in);

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_H | GB_FLAG_C);
    GB_FLAG_SET(GB_FLAG_N);

    uint8_t low_a = (uint8_t)(a & 0x0Fu);
    uint8_t low_v = (uint8_t)(value & 0x0Fu);

    if (low_a < (uint8_t)(low_v + carry_in))
        GB_FLAG_SET(GB_FLAG_H);
    if ((uint16_t)a < (uint16_t)value + carry_in)
        GB_FLAG_SET(GB_FLAG_C);
    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.registers.r8.a = result;
    gb_proc.cycles += 1;
}

void OP_0b10011110(void)
{
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t value = mem_read(gb_proc.registers.r16.hl);
    uint8_t carry_in = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;

    uint8_t result = (uint8_t)(a - value - carry_in);

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_H | GB_FLAG_C);
    GB_FLAG_SET(GB_FLAG_N);

    uint8_t low_a = (uint8_t)(a & 0x0Fu);
    uint8_t low_v = (uint8_t)(value & 0x0Fu);

    if (low_a < (uint8_t)(low_v + carry_in))
        GB_FLAG_SET(GB_FLAG_H);
    if ((uint16_t)a < (uint16_t)value + carry_in)
        GB_FLAG_SET(GB_FLAG_C);
    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.registers.r8.a = result;
    gb_proc.cycles += 2;
}

void OP_0b10100xxx(void)
{
    uint8_t src_idx = gb_proc.opcode & 0x07u;
    uint8_t value = *reg8_table[src_idx];
    uint8_t result = (uint8_t)(gb_proc.registers.r8.a & value);

    gb_proc.registers.r8.a = result;

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_C | GB_FLAG_Z);
    GB_FLAG_SET(GB_FLAG_H);
    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.cycles += 1;
}