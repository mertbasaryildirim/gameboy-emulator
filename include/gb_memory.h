#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    uint8_t mem[0x10000];

    uint8_t boot_rom[0x100];
    uint8_t boot_rom_enabled;

    uint8_t *cart_rom;
    size_t cart_rom_size;

    uint8_t *cart_ram;
    size_t cart_ram_size;
} GB_MEMORY;

extern GB_MEMORY gb;

void memory_init();
void memory_reset();

uint8_t mem_read(uint16_t addr);
void mem_write(uint16_t addr, uint8_t value);

void load_boot_rom(const char *path);
void load_cartridge(const char *path);

#endif