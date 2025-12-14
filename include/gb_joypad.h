#ifndef GB_JOYPAD_H
#define GB_JOYPAD_H

#include <stdint.h>

typedef enum
{
    GB_JOY_RIGHT = 0,
    GB_JOY_LEFT,
    GB_JOY_UP,
    GB_JOY_DOWN,
    GB_JOY_A,
    GB_JOY_B,
    GB_JOY_SELECT,
    GB_JOY_START
} GB_JOYPAD_BUTTON;

void gb_joypad_init(void);
void gb_joypad_reset(void);

uint8_t gb_joypad_read(void);
void gb_joypad_write(uint8_t value);

void gb_joypad_set_button(GB_JOYPAD_BUTTON button, uint8_t pressed);

#endif