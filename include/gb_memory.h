#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

typedef enum
{
    GB_MBC_NONE = 0,
    GB_MBC_MBC1 = 1,
    GB_MBC_MBC3 = 2,
    GB_MBC_MBC5 = 3,
    GB_MBC_UNKNOWN = 0xFF
} GB_MBC_TYPE;

typedef struct
{
    uint8_t mem[0x10000];

    uint8_t boot_rom[0x100];
    uint8_t boot_rom_enabled;

    uint8_t *cart_rom;
    size_t cart_rom_size;

    uint8_t *cart_ram;
    size_t cart_ram_size;

    uint8_t cart_type;
    GB_MBC_TYPE mbc_type;
    uint16_t rom_bank_count;
    uint16_t ram_bank_count;

    uint8_t ram_enabled;

    uint8_t mbc1_rom_bank_low5;
    uint8_t mbc1_rom_bank_high2;
    uint8_t mbc1_ram_bank;
    uint8_t mbc1_banking_mode;

    uint8_t mbc3_rom_bank;
    uint8_t mbc3_ram_rtc_select;
    uint8_t mbc3_rtc_regs[5];
    uint8_t mbc3_rtc_latched_regs[5];
    uint8_t mbc3_rtc_latch_state;

    uint16_t mbc5_rom_bank;
    uint8_t mbc5_ram_bank;
} GB_MEMORY;

extern GB_MEMORY gb;

void memory_init(void);
void memory_reset(void);

uint8_t mem_read(uint16_t addr);
void mem_write(uint16_t addr, uint8_t value);

void load_boot_rom(const char *path);
void load_cartridge(const char *path);

#endif
