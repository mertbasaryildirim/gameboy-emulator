#include "gb_memory.h"
#include <stdio.h>
#include <stdlib.h>

GB_MEMORY gb;

uint8_t mem_read(uint16_t addr)
{
    if (gb.boot_rom_enabled && addr < 0x0100)
        return gb.boot_rom[addr];

    return gb.mem[addr];
}

void mem_write(uint16_t addr, uint8_t v)
{
    if (addr == 0xFF04)
        gb.mem[0xFF04] = 0;
    else if (addr == 0xFF44)
        gb.mem[0xFF44] = 0;
    else if (addr == 0xFF46)
        gb.mem[0xFF46] = 0;
    else
        gb.mem[addr] = v;
}

void memory_init()
{
    memset(&gb, 0, sizeof(GB_MEMORY));
    gb.boot_rom_enabled = 1;
}

void memory_reset()
{
    memset(gb.mem, 0, sizeof(gb.mem));
    gb.boot_rom_enabled = 1;
}

void load_boot_rom(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp)
        return;
    fread(gb.boot_rom, 1, 0x100, fp);
    fclose(fp);
}

void load_cartridge(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp)
        return;

    fseek(fp, 0, SEEK_END);
    gb.cart_rom_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    gb.cart_rom = malloc(gb.cart_rom_size);
    fread(gb.cart_rom, 1, gb.cart_rom_size, fp);
    fclose(fp);

    memcpy(gb.mem, gb.cart_rom, gb.cart_rom_size < 0x8000 ? gb.cart_rom_size : 0x8000);
}
