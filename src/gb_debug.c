#include <stdio.h>

#include "gb_debug.h"
#include "gb_processor.h"
#include "gb_ppu.h"
#include "memory_map.h"

static int gb_debug_enabled = 0;

void gb_debug_init(void)
{
    gb_debug_enabled = 0;
}

void gb_debug_toggle(void)
{
    gb_debug_enabled = !gb_debug_enabled;
}

void gb_debug_frame(void)
{
    if (!gb_debug_enabled)
        return;

    printf(
        "PC=%04X AF=%04X BC=%04X DE=%04X HL=%04X SP=%04X LY=%3u STAT=%02X cycles=%llu\n",
        gb_proc.pc,
        gb_proc.registers.r16.af,
        gb_proc.registers.r16.bc,
        gb_proc.registers.r16.de,
        gb_proc.registers.r16.hl,
        gb_proc.registers.r16.sp,
        (unsigned)IO_LY,
        (unsigned)IO_STAT,
        (unsigned long long)gb_proc.cycles);
}