#include "gb_joypad.h"
#include "memory_map.h"
#include "gb_processor.h"

static uint8_t jp_select;
static uint8_t jp_directions;
static uint8_t jp_buttons;

static uint8_t gb_joypad_compute_value(void)
{
    uint8_t value;
    uint8_t lower;
    uint8_t dir_selected = (uint8_t)((jp_select & 0x10u) == 0u);
    uint8_t btn_selected = (uint8_t)((jp_select & 0x20u) == 0u);

    lower = 0x0Fu;

    if (dir_selected)
        lower = (uint8_t)(lower & (uint8_t)~jp_directions);

    if (btn_selected)
        lower = (uint8_t)(lower & (uint8_t)~jp_buttons);

    value = (uint8_t)(0xC0u | (jp_select & 0x30u) | (lower & 0x0Fu));
    IO_PI = value;
    return value;
}

void gb_joypad_init(void)
{
    jp_select = 0x30u;
    jp_directions = 0u;
    jp_buttons = 0u;
    gb_joypad_compute_value();
}

void gb_joypad_reset(void)
{
    jp_select = 0x30u;
    jp_directions = 0u;
    jp_buttons = 0u;
    gb_joypad_compute_value();
}

uint8_t gb_joypad_read(void)
{
    return gb_joypad_compute_value();
}

void gb_joypad_write(uint8_t value)
{
    jp_select = (uint8_t)(value & 0x30u);
    gb_joypad_compute_value();
}

void gb_joypad_set_button(GB_JOYPAD_BUTTON button, uint8_t pressed)
{
    uint8_t old_value = gb_joypad_compute_value();

    switch (button)
    {
    case GB_JOY_RIGHT:
        if (pressed)
            jp_directions |= 0x01u;
        else
            jp_directions &= (uint8_t)~0x01u;
        break;
    case GB_JOY_LEFT:
        if (pressed)
            jp_directions |= 0x02u;
        else
            jp_directions &= (uint8_t)~0x02u;
        break;
    case GB_JOY_UP:
        if (pressed)
            jp_directions |= 0x04u;
        else
            jp_directions &= (uint8_t)~0x04u;
        break;
    case GB_JOY_DOWN:
        if (pressed)
            jp_directions |= 0x08u;
        else
            jp_directions &= (uint8_t)~0x08u;
        break;
    case GB_JOY_A:
        if (pressed)
            jp_buttons |= 0x01u;
        else
            jp_buttons &= (uint8_t)~0x01u;
        break;
    case GB_JOY_B:
        if (pressed)
            jp_buttons |= 0x02u;
        else
            jp_buttons &= (uint8_t)~0x02u;
        break;
    case GB_JOY_SELECT:
        if (pressed)
            jp_buttons |= 0x04u;
        else
            jp_buttons &= (uint8_t)~0x04u;
        break;
    case GB_JOY_START:
        if (pressed)
            jp_buttons |= 0x08u;
        else
            jp_buttons &= (uint8_t)~0x08u;
        break;
    default:
        break;
    }

    {
        uint8_t new_value = gb_joypad_compute_value();
        uint8_t old_low = (uint8_t)(old_value & 0x0Fu);
        uint8_t new_low = (uint8_t)(new_value & 0x0Fu);
        uint8_t falling = (uint8_t)(old_low & (uint8_t)~new_low);

        if (falling != 0u)
            IO_IF |= GB_INT_JOYPAD;
    }
}