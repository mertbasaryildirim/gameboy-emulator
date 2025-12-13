#include "gb_processor.h"
#include <string.h>

#include "gb_memory.h"
#include "memory_map.h"
#include "instruction_table1.h"
#include "gb_timer.h"

GB_PROC gb_proc;

static uint8_t cpu_fetch_byte(void)
{
    uint8_t v;

    if (gb_proc.halt_bug)
    {
        gb_proc.halt_bug = 0;
        v = mem_read(gb_proc.pc);
        return v;
    }

    v = mem_read(gb_proc.pc);
    gb_proc.pc++;
    return v;
}

static void cpu_handle_interrupt(uint8_t bit, uint16_t vector)
{
    uint16_t pc;

    gb_proc.ime = 0;
    gb_proc.ime_enable_pending = 0;
    gb_proc.halted = 0;
    gb_proc.stopped = 0;

    IO_IF &= (uint8_t)~(uint8_t)(1u << bit);

    pc = gb_proc.pc;
    gb_proc.registers.r16.sp -= 2u;

    mem_write(gb_proc.registers.r16.sp, (uint8_t)(pc & 0xFFu));
    mem_write((uint16_t)(gb_proc.registers.r16.sp + 1u), (uint8_t)(pc >> 8));

    gb_proc.pc = vector;

    gb_proc.cycles += 5u;
    gb_timer_step(5u);
}

static uint8_t cpu_handle_interrupts(void)
{
    uint8_t pending;

    pending = (uint8_t)(IO_IF & IO_IE);
    if (pending == 0u)
        return 0u;

    if (!gb_proc.ime)
    {
        if (gb_proc.halted)
        {
            gb_proc.halted = 0;
            gb_proc.halt_bug = 1;
        }
        return 0u;
    }

    if (pending & GB_INT_VBLANK)
    {
        cpu_handle_interrupt(0u, 0x0040u);
        return 1u;
    }
    if (pending & GB_INT_STAT)
    {
        cpu_handle_interrupt(1u, 0x0048u);
        return 1u;
    }
    if (pending & GB_INT_TIMER)
    {
        cpu_handle_interrupt(2u, 0x0050u);
        return 1u;
    }
    if (pending & GB_INT_SERIAL)
    {
        cpu_handle_interrupt(3u, 0x0058u);
        return 1u;
    }
    if (pending & GB_INT_JOYPAD)
    {
        cpu_handle_interrupt(4u, 0x0060u);
        return 1u;
    }

    return 0u;
}

void gb_cpu_init(void)
{
    memset(&gb_proc, 0, sizeof(GB_PROC));
}

void gb_cpu_reset(void)
{
    memset(&gb_proc, 0, sizeof(GB_PROC));
}

void gb_cpu_step(void)
{
    uint64_t old_cycles;
    uint8_t opcode;
    instruction1_handler_t handler;

    if (gb_proc.ime_enable_pending)
    {
        gb_proc.ime = 1;
        gb_proc.ime_enable_pending = 0;
    }

    if (cpu_handle_interrupts())
        return;

    if (gb_proc.stopped)
    {
        gb_proc.cycles += 1u;
        gb_timer_step(1u);
        return;
    }

    if (gb_proc.halted)
    {
        gb_proc.cycles += 1u;
        gb_timer_step(1u);
        return;
    }

    opcode = cpu_fetch_byte();
    gb_proc.opcode = opcode;

    handler = instruction1_get_handler(opcode);
    if (handler)
    {
        old_cycles = gb_proc.cycles;
        handler();
        gb_timer_step((uint8_t)(gb_proc.cycles - old_cycles));
    }
    else
    {
        gb_proc.cycles += 1u;
        gb_timer_step(1u);
    }
}