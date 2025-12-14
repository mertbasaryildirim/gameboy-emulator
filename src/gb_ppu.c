#include "gb_ppu.h"
#include "gb_memory.h"
#include "memory_map.h"
#include "gb_processor.h"
#include <string.h>

GB_PPU gb_ppu;

static const uint32_t dmg_palette_table[4] = {
    0xFFFFFFFFu,
    0xFFAAAAAAu,
    0xFF555555u,
    0xFF000000u};

static uint8_t palette_map(uint8_t pal, uint8_t color_id)
{
    uint8_t shift = (uint8_t)(color_id * 2u);
    return (uint8_t)((pal >> shift) & 0x03u);
}

static void ppu_set_mode(uint8_t mode)
{
    uint8_t stat = IO_STAT;
    uint8_t old_mode = (uint8_t)(stat & 0x03u);

    if (old_mode == mode)
        return;

    stat = (uint8_t)((stat & (uint8_t)~0x03u) | mode);
    IO_STAT = stat;

    if (mode == 0u)
    {
        if (stat & 0x08u)
            IO_IF |= GB_INT_STAT;
    }
    else if (mode == 1u)
    {
        if (stat & 0x10u)
            IO_IF |= GB_INT_STAT;
    }
    else if (mode == 2u)
    {
        if (stat & 0x20u)
            IO_IF |= GB_INT_STAT;
    }

    gb_ppu.mode = mode;
}

static void ppu_update_lyc(void)
{
    if (IO_LY == IO_LCY)
    {
        if ((IO_STAT & 0x04u) == 0u)
        {
            IO_STAT |= 0x04u;
            if (IO_STAT & 0x40u)
                IO_IF |= GB_INT_STAT;
        }
    }
    else
    {
        IO_STAT &= (uint8_t)~0x04u;
    }
}

static void ppu_render_scanline(uint8_t ly)
{
    uint8_t lcdc;
    uint8_t bg_enable;
    uint8_t window_enable;
    uint8_t obj_enable;
    uint8_t obj_size_flag;
    uint8_t bg_tilemap_select;
    uint8_t window_tilemap_select;
    uint8_t tiledata_signed;
    uint8_t scx;
    uint8_t scy;
    uint8_t wy;
    uint8_t wx;
    uint8_t bgp;
    uint8_t obp0;
    uint8_t obp1;
    uint8_t bg_color_id[GB_LCD_WIDTH];
    uint8_t final_shade[GB_LCD_WIDTH];
    uint8_t x;

    if (ly >= GB_LCD_HEIGHT)
        return;

    lcdc = IO_LCDC;
    if ((lcdc & 0x80u) == 0u)
    {
        for (x = 0; x < GB_LCD_WIDTH; ++x)
            gb_ppu.framebuffer[ly * GB_LCD_WIDTH + x] = dmg_palette_table[0];
        return;
    }

    bg_enable = (uint8_t)(lcdc & 0x01u);
    window_enable = (uint8_t)(lcdc & 0x20u);
    obj_enable = (uint8_t)(lcdc & 0x02u);
    obj_size_flag = (uint8_t)(lcdc & 0x04u);
    bg_tilemap_select = (uint8_t)(lcdc & 0x08u);
    window_tilemap_select = (uint8_t)(lcdc & 0x40u);
    tiledata_signed = (uint8_t)((lcdc & 0x10u) == 0u);

    scx = IO_SCX;
    scy = IO_SCY;
    wy = IO_WY;
    wx = IO_WX;

    bgp = IO_BGP;
    obp0 = IO_OBP0;
    obp1 = IO_OBP1;

    if (bg_enable)
    {
        uint16_t bg_map_base = bg_tilemap_select ? 0x9C00u : 0x9800u;
        uint8_t y_in_bg = (uint8_t)(ly + scy);
        uint8_t tile_y = (uint8_t)(y_in_bg >> 3);

        for (x = 0; x < GB_LCD_WIDTH; ++x)
        {
            uint8_t x_in_bg = (uint8_t)(x + scx);
            uint8_t tile_x = (uint8_t)(x_in_bg >> 3);
            uint16_t map_addr = (uint16_t)(bg_map_base + (uint16_t)(tile_y * 32u) + tile_x);
            uint8_t tile_index = gb.mem[map_addr];
            uint16_t tile_addr;
            uint8_t line_in_tile = (uint8_t)(y_in_bg & 0x07u);
            uint8_t low;
            uint8_t high;
            uint8_t bit;
            uint8_t lo;
            uint8_t hi;

            if (tiledata_signed)
            {
                int8_t sindex = (int8_t)tile_index;
                tile_addr = (uint16_t)(0x9000 + (int16_t)sindex * 16);
            }
            else
            {
                tile_addr = (uint16_t)(0x8000 + (uint16_t)tile_index * 16u);
            }

            low = gb.mem[(uint16_t)(tile_addr + (uint16_t)line_in_tile * 2u)];
            high = gb.mem[(uint16_t)(tile_addr + (uint16_t)line_in_tile * 2u + 1u)];
            bit = (uint8_t)(7u - (x_in_bg & 0x07u));
            lo = (uint8_t)((low >> bit) & 0x01u);
            hi = (uint8_t)((high >> bit) & 0x01u);
            bg_color_id[x] = (uint8_t)((hi << 1u) | lo);
        }
    }
    else
    {
        for (x = 0; x < GB_LCD_WIDTH; ++x)
            bg_color_id[x] = 0u;
    }

    if (window_enable)
    {
        uint8_t window_y_cond = (uint8_t)(ly >= wy);
        if (window_y_cond && wx <= 166u)
        {
            uint16_t win_map_base = window_tilemap_select ? 0x9C00u : 0x9800u;
            uint8_t win_y = (uint8_t)(ly - wy);
            uint8_t tile_y = (uint8_t)(win_y >> 3);
            uint8_t window_x0 = (uint8_t)(wx >= 7u ? (uint8_t)(wx - 7u) : 0u);

            for (x = 0; x < GB_LCD_WIDTH; ++x)
            {
                if (x >= window_x0)
                {
                    uint8_t win_x = (uint8_t)(x - window_x0);
                    uint8_t tile_x = (uint8_t)(win_x >> 3);
                    uint16_t map_addr = (uint16_t)(win_map_base + (uint16_t)(tile_y * 32u) + tile_x);
                    uint8_t tile_index = gb.mem[map_addr];
                    uint16_t tile_addr;
                    uint8_t line_in_tile = (uint8_t)(win_y & 0x07u);
                    uint8_t low;
                    uint8_t high;
                    uint8_t bit;
                    uint8_t lo;
                    uint8_t hi;

                    if (tiledata_signed)
                    {
                        int8_t sindex = (int8_t)tile_index;
                        tile_addr = (uint16_t)(0x9000 + (int16_t)sindex * 16);
                    }
                    else
                    {
                        tile_addr = (uint16_t)(0x8000 + (uint16_t)tile_index * 16u);
                    }

                    low = gb.mem[(uint16_t)(tile_addr + (uint16_t)line_in_tile * 2u)];
                    high = gb.mem[(uint16_t)(tile_addr + (uint16_t)line_in_tile * 2u + 1u)];
                    bit = (uint8_t)(7u - (win_x & 0x07u));
                    lo = (uint8_t)((low >> bit) & 0x01u);
                    hi = (uint8_t)((high >> bit) & 0x01u);
                    bg_color_id[x] = (uint8_t)((hi << 1u) | lo);
                }
            }
        }
    }

    for (x = 0; x < GB_LCD_WIDTH; ++x)
    {
        uint8_t cid = bg_color_id[x];
        uint8_t shade = palette_map(bgp, cid);
        final_shade[x] = shade;
    }

    if (obj_enable)
    {
        uint8_t obj_height = obj_size_flag ? 16u : 8u;
        uint8_t sprites_on_line = 0u;
        uint8_t i;

        for (i = 0; i < 40u; ++i)
        {
            uint16_t oam_addr = (uint16_t)(0xFE00u + (uint16_t)i * 4u);
            uint8_t y = gb.mem[oam_addr + 0u];
            uint8_t x_pos = gb.mem[oam_addr + 1u];
            uint8_t tile_index = gb.mem[oam_addr + 2u];
            uint8_t attr = gb.mem[oam_addr + 3u];
            int16_t sprite_y;
            int16_t sprite_x;
            int16_t line;

            if (y == 0u || y >= 160u + 16u)
                continue;

            sprite_y = (int16_t)y - 16;
            if ((int16_t)ly < sprite_y || (int16_t)ly >= (sprite_y + (int16_t)obj_height))
                continue;

            if (sprites_on_line >= 10u)
                continue;

            sprite_x = (int16_t)x_pos - 8;
            line = (int16_t)ly - sprite_y;

            if (attr & 0x40u)
                line = (int16_t)(obj_height - 1 - line);

            {
                uint8_t tile_row;
                uint8_t row_in_tile;
                uint16_t tile_addr;
                uint8_t low;
                uint8_t high;
                uint8_t px;

                if (obj_height == 16u)
                {
                    uint8_t base_index = (uint8_t)(tile_index & 0xFEu);
                    if (line >= 8)
                        tile_row = (uint8_t)(base_index + 1u);
                    else
                        tile_row = base_index;
                    row_in_tile = (uint8_t)(line & 0x07u);
                    tile_addr = (uint16_t)(0x8000u + (uint16_t)tile_row * 16u);
                }
                else
                {
                    tile_row = tile_index;
                    row_in_tile = (uint8_t)(line & 0x07u);
                    tile_addr = (uint16_t)(0x8000u + (uint16_t)tile_row * 16u);
                }

                low = gb.mem[(uint16_t)(tile_addr + (uint16_t)row_in_tile * 2u)];
                high = gb.mem[(uint16_t)(tile_addr + (uint16_t)row_in_tile * 2u + 1u)];

                for (px = 0; px < 8u; ++px)
                {
                    int16_t screen_x;
                    uint8_t bit_index;
                    uint8_t lo;
                    uint8_t hi;
                    uint8_t color_id;
                    uint8_t bg_id;
                    uint8_t pal;
                    uint8_t shade;

                    if (attr & 0x20u)
                        screen_x = (int16_t)sprite_x + (int16_t)(7u - px);
                    else
                        screen_x = (int16_t)sprite_x + (int16_t)px;

                    if (screen_x < 0 || screen_x >= GB_LCD_WIDTH)
                        continue;

                    bit_index = (uint8_t)(7u - px);
                    lo = (uint8_t)((low >> bit_index) & 0x01u);
                    hi = (uint8_t)((high >> bit_index) & 0x01u);
                    color_id = (uint8_t)((hi << 1u) | lo);

                    if (color_id == 0u)
                        continue;

                    bg_id = bg_color_id[(uint8_t)screen_x];

                    if ((attr & 0x80u) && bg_id != 0u)
                        continue;

                    pal = (attr & 0x10u) ? obp1 : obp0;
                    shade = palette_map(pal, color_id);
                    final_shade[(uint8_t)screen_x] = shade;
                }
            }

            sprites_on_line++;
        }
    }

    for (x = 0; x < GB_LCD_WIDTH; ++x)
    {
        uint8_t shade = final_shade[x];
        gb_ppu.framebuffer[ly * GB_LCD_WIDTH + x] = dmg_palette_table[shade & 0x03u];
    }
}

void gb_ppu_init(void)
{
    memset(&gb_ppu, 0, sizeof(GB_PPU));
}

void gb_ppu_reset(void)
{
    memset(&gb_ppu, 0, sizeof(GB_PPU));
}

void gb_ppu_clear_frame_complete(void)
{
    gb_ppu.frame_complete = 0;
}

void gb_ppu_step(uint8_t m_cycles)
{
    uint16_t t_cycles = (uint16_t)m_cycles * 4u;

    while (t_cycles-- > 0u)
    {
        uint8_t lcdc = IO_LCDC;

        if ((lcdc & 0x80u) == 0u)
        {
            IO_LY = 0u;
            gb_ppu.dot = 0u;
            gb_ppu.mode = 0u;
            gb_ppu.lcd_on_prev = 0u;
            gb_ppu.frame_complete = 0u;
            IO_STAT &= (uint8_t)~0x03u;
            ppu_update_lyc();
            continue;
        }

        if (!gb_ppu.lcd_on_prev)
        {
            gb_ppu.lcd_on_prev = 1u;
            gb_ppu.dot = 0u;
            IO_LY = 0u;
            gb_ppu.frame_complete = 0u;
            ppu_set_mode(2u);
            ppu_update_lyc();
        }

        {
            uint8_t ly = IO_LY;
            uint16_t dot = gb_ppu.dot;
            uint8_t new_mode;

            if (ly >= 144u)
                new_mode = 1u;
            else if (dot < 80u)
                new_mode = 2u;
            else if (dot < 252u)
                new_mode = 3u;
            else
                new_mode = 0u;

            if (new_mode != gb_ppu.mode)
            {
                if (new_mode == 3u && ly < 144u)
                    ppu_render_scanline(ly);

                ppu_set_mode(new_mode);
            }

            ppu_update_lyc();

            gb_ppu.dot++;
            if (gb_ppu.dot >= 456u)
            {
                gb_ppu.dot = 0u;
                IO_LY++;

                if (IO_LY == 144u)
                {
                    ppu_set_mode(1u);
                    IO_IF |= GB_INT_VBLANK;
                    if (IO_STAT & 0x10u)
                        IO_IF |= GB_INT_STAT;
                }
                else if (IO_LY > 153u)
                {
                    IO_LY = 0u;
                    gb_ppu.frame_complete = 1u;
                }

                ppu_update_lyc();
            }
        }
    }
}