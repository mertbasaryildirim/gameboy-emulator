#include "gb_timer.h"
#include "memory_map.h"
#include "gb_processor.h"

static uint16_t div_counter;

static uint8_t timer_is_enabled(void)
{
    return (uint8_t)((IO_TAC & 0x04u) != 0u);
}

static uint8_t timer_clock_bit(void)
{
    switch (IO_TAC & 0x03u)
    {
    case 0x00u:
        return 9u;
    case 0x01u:
        return 3u;
    case 0x02u:
        return 5u;
    case 0x03u:
    default:
        return 7u;
    }
}

void gb_timer_init(void)
{
    div_counter = 0u;
    IO_DIV = 0u;
    IO_TIMA = 0u;
    IO_TMA = 0u;
    IO_TAC = 0u;
}

void gb_timer_reset(void)
{
    div_counter = 0u;
    IO_DIV = 0u;
    IO_TIMA = 0u;
    IO_TMA = 0u;
    IO_TAC = 0u;
}

void gb_timer_div_reset(void)
{
    div_counter = 0u;
    IO_DIV = 0u;
}

void gb_timer_step(uint8_t m_cycles)
{
    uint16_t t_cycles;
    uint8_t enabled;
    uint8_t bit_index;

    t_cycles = (uint16_t)m_cycles * 4u;

    while (t_cycles-- > 0u)
    {
        uint16_t old_counter;
        uint8_t old_bit;
        uint8_t new_bit;

        old_counter = div_counter;
        div_counter++;

        IO_DIV = (uint8_t)(div_counter >> 8);

        enabled = timer_is_enabled();
        if (!enabled)
            continue;

        bit_index = timer_clock_bit();

        old_bit = (uint8_t)((old_counter >> bit_index) & 0x01u);
        new_bit = (uint8_t)((div_counter >> bit_index) & 0x01u);

        if (old_bit == 1u && new_bit == 0u)
        {
            uint8_t tima = IO_TIMA;
            if (tima == 0xFFu)
            {
                IO_TIMA = IO_TMA;
                IO_IF |= GB_INT_TIMER;
            }
            else
            {
                IO_TIMA = (uint8_t)(tima + 1u);
            }
        }
    }
}