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

void OP_0b00110100(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t value = mem_read(addr);
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

    mem_write(addr, result);
    gb_proc.cycles += 3;
}

void OP_0b00110101(void)
{
    uint16_t addr = gb_proc.registers.r16.hl;
    uint8_t value = mem_read(addr);
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

    mem_write(addr, result);
    gb_proc.cycles += 3;
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

void OP_0b01xxxyyy(void)
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

void OP_0b10100110(void)
{
    uint8_t value = mem_read(gb_proc.registers.r16.hl);
    uint8_t result = (uint8_t)(gb_proc.registers.r8.a & value);

    gb_proc.registers.r8.a = result;

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_C | GB_FLAG_Z);
    GB_FLAG_SET(GB_FLAG_H);
    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.cycles += 2;
}

void OP_0b10101xxx(void)
{
    uint8_t idx = gb_proc.opcode & 0x07u;
    uint8_t value = *reg8_table[idx];
    uint8_t result = (uint8_t)(gb_proc.registers.r8.a ^ value);

    gb_proc.registers.r8.a = result;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);
    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.cycles += 1;
}

void OP_0b10101110(void)
{
    uint8_t value = mem_read(gb_proc.registers.r16.hl);
    uint8_t result = (uint8_t)(gb_proc.registers.r8.a ^ value);

    gb_proc.registers.r8.a = result;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);
    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.cycles += 2;
}

void OP_0b10110xxx(void)
{
    uint8_t idx = gb_proc.opcode & 0x07u;
    uint8_t value = *reg8_table[idx];
    uint8_t result = (uint8_t)(gb_proc.registers.r8.a | value);

    gb_proc.registers.r8.a = result;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);
    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.cycles += 1;
}

void OP_0b10110110(void)
{
    uint8_t value = mem_read(gb_proc.registers.r16.hl);
    uint8_t result = (uint8_t)(gb_proc.registers.r8.a | value);

    gb_proc.registers.r8.a = result;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);
    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);

    gb_proc.cycles += 2;
}

void OP_0b10111xxx(void)
{
    uint8_t idx = gb_proc.opcode & 0x07u;
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t value = *reg8_table[idx];
    uint8_t res = (uint8_t)(a - value);

    GB_FLAG_SET(GB_FLAG_N);
    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_H | GB_FLAG_C);

    if (res == 0)
        GB_FLAG_SET(GB_FLAG_Z);
    if ((a & 0x0Fu) < (value & 0x0Fu))
        GB_FLAG_SET(GB_FLAG_H);
    if (a < value)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 1;
}

void OP_0b10111110(void)
{
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t value = mem_read(gb_proc.registers.r16.hl);
    uint8_t res = (uint8_t)(a - value);

    GB_FLAG_SET(GB_FLAG_N);
    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_H | GB_FLAG_C);

    if (res == 0)
        GB_FLAG_SET(GB_FLAG_Z);
    if ((a & 0x0Fu) < (value & 0x0Fu))
        GB_FLAG_SET(GB_FLAG_H);
    if (a < value)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_0b110xx000(void)
{
    uint8_t cc = (gb_proc.opcode >> 3) & 0x03u;
    uint8_t z = (gb_proc.registers.r8.f & GB_FLAG_Z) != 0;
    uint8_t c = (gb_proc.registers.r8.f & GB_FLAG_C) != 0;
    uint8_t flag = (cc & 0x02u) ? c : z;
    uint8_t take = (cc & 0x01u) ? flag : (uint8_t)!flag;

    if (take)
    {
        uint8_t lo = mem_read(gb_proc.registers.r16.sp++);
        uint8_t hi = mem_read(gb_proc.registers.r16.sp++);
        uint16_t ret = (uint16_t)lo | ((uint16_t)hi << 8);

        gb_proc.pc = (uint16_t)(ret - 1u);
        gb_proc.cycles += 5;
    }
    else
    {
        gb_proc.cycles += 2;
    }
}

void OP_0b11xx0001(void)
{
    uint8_t xx = (gb_proc.opcode >> 4) & 0x3u;

    uint16_t sp = gb_proc.registers.r16.sp;
    uint8_t lsb = mem_read(sp++);
    uint8_t msb = mem_read(sp++);
    gb_proc.registers.r16.sp = sp;

    uint16_t value = (uint16_t)lsb | ((uint16_t)msb << 8);

    uint16_t *rr_base = &gb_proc.registers.r16.af;
    uint8_t index = (uint8_t)((xx + 1u) & 0x3u);
    uint16_t *rr = rr_base + index;
    *rr = value;

    if (xx == 3u)
        gb_proc.registers.r8.f &= 0xF0u;

    gb_proc.cycles += 3;
}

void OP_0b110xx010(void)
{
    static const uint8_t cond_masks[4] = {
        GB_FLAG_Z, GB_FLAG_Z, GB_FLAG_C, GB_FLAG_C};
    static const uint8_t cond_true_when_set[4] = {
        0u, 1u, 0u, 1u};

    uint8_t xx = (gb_proc.opcode >> 3) & 0x3u;

    uint8_t nn_l = mem_read(++gb_proc.pc);
    uint8_t nn_h = mem_read(++gb_proc.pc);
    uint16_t nn = (uint16_t)nn_l | ((uint16_t)nn_h << 8);

    uint8_t mask = cond_masks[xx];
    uint8_t want_set = cond_true_when_set[xx];
    uint8_t flag_is_set = (gb_proc.registers.r8.f & mask) != 0u;

    if (flag_is_set == (want_set != 0u))
    {
        gb_proc.pc = (uint16_t)(nn - 1u);
        gb_proc.cycles += 4;
    }
    else
    {
        gb_proc.cycles += 3;
    }
}

void OP_0b11000011(void)
{
    uint8_t nn_l = mem_read(++gb_proc.pc);
    uint8_t nn_h = mem_read(++gb_proc.pc);
    uint16_t nn = (uint16_t)nn_l | ((uint16_t)nn_h << 8);

    gb_proc.pc = (uint16_t)(nn - 1u);
    gb_proc.cycles += 4;
}

void OP_0b110xx100(void)
{
    uint8_t cc = (gb_proc.opcode >> 3) & 0x3u;

    uint8_t nn_l = mem_read(++gb_proc.pc);
    uint8_t nn_h = mem_read(++gb_proc.pc);
    uint16_t nn = (uint16_t)nn_l | ((uint16_t)nn_h << 8);

    static const uint8_t cc_mask[4] = {GB_FLAG_Z, GB_FLAG_Z, GB_FLAG_C, GB_FLAG_C};
    static const uint8_t cc_value[4] = {0u, GB_FLAG_Z, 0u, GB_FLAG_C};

    uint8_t f = gb_proc.registers.r8.f;

    if ((f & cc_mask[cc]) == cc_value[cc])
    {
        uint16_t ret = (uint16_t)(gb_proc.pc + 1u);
        uint16_t sp = gb_proc.registers.r16.sp;

        sp -= 1;
        mem_write(sp, (uint8_t)(ret >> 8));
        sp -= 1;
        mem_write(sp, (uint8_t)(ret & 0xFFu));

        gb_proc.registers.r16.sp = sp;
        gb_proc.pc = (uint16_t)(nn - 1u);

        gb_proc.cycles += 6;
    }
    else
    {
        gb_proc.cycles += 3;
    }
}

void OP_0b11xx0101(void)
{
    uint8_t xx = (gb_proc.opcode >> 4) & 0x3u;

    uint16_t *rr_base = &gb_proc.registers.r16.af;
    uint8_t index = (uint8_t)((xx + 1u) & 0x3u);
    uint16_t value = rr_base[index];

    if (xx == 3u)
        value &= 0xFFF0u;

    gb_proc.registers.r16.sp--;
    mem_write(gb_proc.registers.r16.sp, (uint8_t)(value >> 8));

    gb_proc.registers.r16.sp--;
    mem_write(gb_proc.registers.r16.sp, (uint8_t)(value & 0xFFu));

    gb_proc.cycles += 4;
}

void OP_0b11000110(void)
{
    uint8_t n = mem_read(++gb_proc.pc);

    uint8_t a = gb_proc.registers.r8.a;
    uint16_t sum = (uint16_t)a + (uint16_t)n;
    uint8_t result = (uint8_t)sum;

    if (result == 0u)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N);

    if (((a ^ n ^ result) & 0x10u) != 0u)
        GB_FLAG_SET(GB_FLAG_H);
    else
        GB_FLAG_CLEAR(GB_FLAG_H);

    if (sum & 0x100u)
        GB_FLAG_SET(GB_FLAG_C);
    else
        GB_FLAG_CLEAR(GB_FLAG_C);

    gb_proc.registers.r8.a = result;
    gb_proc.cycles += 2;
}

void OP_0b11xxx111(void)
{
    uint16_t ret = (uint16_t)(gb_proc.pc + 1u);

    gb_proc.registers.r16.sp--;
    mem_write(gb_proc.registers.r16.sp, (uint8_t)(ret >> 8));
    gb_proc.registers.r16.sp--;
    mem_write(gb_proc.registers.r16.sp, (uint8_t)ret);

    uint16_t vec = (uint16_t)(gb_proc.opcode & 0x38u);
    gb_proc.pc = (uint16_t)(vec - 1u);

    gb_proc.cycles += 4;
}

void OP_0b11001001(void)
{
    uint8_t lsb = mem_read(gb_proc.registers.r16.sp++);
    uint8_t msb = mem_read(gb_proc.registers.r16.sp++);
    uint16_t ret = (uint16_t)lsb | ((uint16_t)msb << 8);

    gb_proc.pc = (uint16_t)(ret - 1u);
    gb_proc.cycles += 4;
}

void OP_0b11001101(void)
{
    uint8_t lsb = mem_read(++gb_proc.pc);
    uint8_t msb = mem_read(++gb_proc.pc);
    uint16_t nn = (uint16_t)lsb | ((uint16_t)msb << 8);

    uint16_t ret = (uint16_t)(gb_proc.pc + 1u);

    uint16_t sp = gb_proc.registers.r16.sp;
    sp--;
    mem_write(sp, (uint8_t)(ret >> 8));
    sp--;
    mem_write(sp, (uint8_t)(ret & 0xFFu));
    gb_proc.registers.r16.sp = sp;

    gb_proc.pc = (uint16_t)(nn - 1u);
    gb_proc.cycles += 6;
}

void OP_0b11001110(void)
{
    uint8_t n = mem_read(++gb_proc.pc);
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t carry_in = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;

    uint16_t sum = (uint16_t)a + (uint16_t)n + (uint16_t)carry_in;
    uint8_t result = (uint8_t)sum;

    gb_proc.registers.r8.a = result;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);
    if (((a & 0x0Fu) + (n & 0x0Fu) + carry_in) > 0x0Fu)
        GB_FLAG_SET(GB_FLAG_H);
    if (sum > 0xFFu)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_0b11010110(void)
{
    uint8_t n = mem_read(++gb_proc.pc);
    alu_sub(n);
    gb_proc.cycles += 2;
}

void OP_0b11011001(void)
{
    uint8_t lsb = mem_read(gb_proc.registers.r16.sp++);
    uint8_t msb = mem_read(gb_proc.registers.r16.sp++);
    uint16_t ret = (uint16_t)lsb | ((uint16_t)msb << 8);

    gb_proc.pc = (uint16_t)(ret - 1u);
    gb_proc.ime = 1;
    gb_proc.cycles += 4;
}

void OP_0b11011110(void)
{
    uint8_t n = mem_read(++gb_proc.pc);
    uint8_t a = gb_proc.registers.r8.a;
    uint8_t carry = GB_FLAG_IS_SET(GB_FLAG_C) ? 1u : 0u;

    uint16_t result16 = (uint16_t)a - (uint16_t)n - (uint16_t)carry;
    uint8_t result8 = (uint8_t)result16;

    gb_proc.registers.r8.a = result8;

    uint8_t f = GB_FLAG_N;

    if (result8 == 0u)
        f |= GB_FLAG_Z;

    if (((a ^ n ^ result16) & 0x10u) != 0u)
        f |= GB_FLAG_H;

    if ((result16 & 0x100u) != 0u)
        f |= GB_FLAG_C;

    gb_proc.registers.r8.f = f;
    gb_proc.cycles += 2;
}

void OP_0b11100000(void)
{
    uint8_t n = mem_read(++gb_proc.pc);
    uint16_t addr = (uint16_t)0xFF00u | (uint16_t)n;
    mem_write(addr, gb_proc.registers.r8.a);
    gb_proc.cycles += 3;
}

void OP_0b11100010(void)
{
    uint16_t addr = (uint16_t)0xFF00u | gb_proc.registers.r8.c;
    mem_write(addr, gb_proc.registers.r8.a);
    gb_proc.cycles += 2;
}

void OP_0b11100110(void)
{
    uint8_t n = mem_read(++gb_proc.pc);
    uint8_t result = gb_proc.registers.r8.a & n;
    gb_proc.registers.r8.a = result;

    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_C);
    GB_FLAG_SET(GB_FLAG_H);

    gb_proc.cycles += 2;
}

void OP_0b11101000(void)
{
    uint8_t e = mem_read(++gb_proc.pc);
    int8_t se = (int8_t)e;

    uint16_t sp = gb_proc.registers.r16.sp;
    uint16_t result = (uint16_t)(sp + se);

    uint16_t sp_low = sp & 0x00FFu;
    uint16_t sum_low = sp_low + (uint16_t)e;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    if ((sp_low ^ (uint16_t)e ^ sum_low) & 0x10u)
        GB_FLAG_SET(GB_FLAG_H);
    if ((sp_low ^ (uint16_t)e ^ sum_low) & 0x100u)
        GB_FLAG_SET(GB_FLAG_C);

    gb_proc.registers.r16.sp = result;
    gb_proc.cycles += 4;
}

void OP_0b11101001(void)
{
    gb_proc.pc = (uint16_t)(gb_proc.registers.r16.hl - 1u);
    gb_proc.cycles += 1;
}

void OP_0b11101010(void)
{
    uint8_t lsb = mem_read(++gb_proc.pc);
    uint8_t msb = mem_read(++gb_proc.pc);
    uint16_t addr = (uint16_t)lsb | ((uint16_t)msb << 8);

    mem_write(addr, gb_proc.registers.r8.a);

    gb_proc.cycles += 4;
}

void OP_0b11101110(void)
{
    uint8_t n = mem_read(++gb_proc.pc);
    uint8_t result = (uint8_t)(gb_proc.registers.r8.a ^ n);

    gb_proc.registers.r8.a = result;

    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_0b11110000(void)
{
    uint8_t n = mem_read(++gb_proc.pc);
    uint16_t addr = (uint16_t)0xFF00u | (uint16_t)n;

    gb_proc.registers.r8.a = mem_read(addr);

    gb_proc.cycles += 3;
}

void OP_0b11110010(void)
{
    uint16_t addr = (uint16_t)(0xFF00u | gb_proc.registers.r8.c);
    gb_proc.registers.r8.a = mem_read(addr);
    gb_proc.cycles += 2;
}

void OP_0b11110011(void)
{
    gb_proc.ime = 0;
    gb_proc.ime_enable_pending = 0;
    gb_proc.cycles += 1;
}

void OP_0b11110110(void)
{
    uint8_t n = mem_read(++gb_proc.pc);
    uint8_t result = (uint8_t)(gb_proc.registers.r8.a | n);

    gb_proc.registers.r8.a = result;

    if (result == 0)
        GB_FLAG_SET(GB_FLAG_Z);
    else
        GB_FLAG_CLEAR(GB_FLAG_Z);

    GB_FLAG_CLEAR(GB_FLAG_N | GB_FLAG_H | GB_FLAG_C);

    gb_proc.cycles += 2;
}

void OP_0b11111000(void)
{
    uint16_t sp = gb_proc.registers.r16.sp;
    int8_t e = (int8_t)mem_read(++gb_proc.pc);

    uint16_t result = (uint16_t)(sp + e);
    gb_proc.registers.r16.hl = result;

    GB_FLAG_CLEAR(GB_FLAG_Z | GB_FLAG_N);

    if (((sp & 0x0Fu) + ((uint16_t)e & 0x0Fu)) > 0x0Fu)
        GB_FLAG_SET(GB_FLAG_H);
    else
        GB_FLAG_CLEAR(GB_FLAG_H);

    if (((sp & 0xFFu) + ((uint16_t)e & 0xFFu)) > 0xFFu)
        GB_FLAG_SET(GB_FLAG_C);
    else
        GB_FLAG_CLEAR(GB_FLAG_C);

    gb_proc.cycles += 3;
}

void OP_0b11111001(void)
{
    gb_proc.registers.r16.sp = gb_proc.registers.r16.hl;
    gb_proc.cycles += 2;
}

void OP_0b11111010(void)
{
    uint8_t lsb = mem_read(++gb_proc.pc);
    uint8_t msb = mem_read(++gb_proc.pc);
    uint16_t addr = (uint16_t)lsb | ((uint16_t)msb << 8);

    gb_proc.registers.r8.a = mem_read(addr);

    gb_proc.cycles += 4;
}

void OP_0b11111011(void)
{
    gb_proc.ime_enable_pending = 1;
    gb_proc.cycles += 1;
}