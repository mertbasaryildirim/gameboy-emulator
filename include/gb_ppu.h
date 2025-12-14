#ifndef GB_PPU_H
#define GB_PPU_H

#include <stdint.h>

#define GB_LCD_WIDTH 160
#define GB_LCD_HEIGHT 144

typedef struct
{
    uint32_t framebuffer[GB_LCD_WIDTH * GB_LCD_HEIGHT];
    uint16_t dot;
    uint8_t mode;
    uint8_t lcd_on_prev;
    uint8_t frame_complete;
} GB_PPU;

extern GB_PPU gb_ppu;

void gb_ppu_init(void);
void gb_ppu_reset(void);
void gb_ppu_step(uint8_t m_cycles);
void gb_ppu_clear_frame_complete(void);

#endif