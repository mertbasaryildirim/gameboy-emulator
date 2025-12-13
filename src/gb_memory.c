#include "gb_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GB_MEMORY gb;

static GB_MBC_TYPE detect_mbc_type(uint8_t cart_type)
{
    switch (cart_type)
    {
    case 0x00:
    case 0x08:
    case 0x09:
        return GB_MBC_NONE;

    case 0x01:
    case 0x02:
    case 0x03:
        return GB_MBC_MBC1;

    default:
        return GB_MBC_UNKNOWN;
    }
}

static uint8_t rom_bank_count_from_header(uint8_t code)
{
    switch (code)
    {
    case 0x00:
        return 2u;
    case 0x01:
        return 4u;
    case 0x02:
        return 8u;
    case 0x03:
        return 16u;
    case 0x04:
        return 32u;
    case 0x05:
        return 64u;
    case 0x06:
        return 128u;
    case 0x07:
        return 256u;
    case 0x52:
        return 72u;
    case 0x53:
        return 80u;
    case 0x54:
        return 96u;
    default:
        return 2u;
    }
}

static uint8_t ram_banks_from_header(uint8_t code)
{
    switch (code)
    {
    case 0x00:
        return 0u;
    case 0x01:
        return 1u;
    case 0x02:
        return 1u;
    case 0x03:
        return 4u;
    case 0x04:
        return 16u;
    default:
        return 0u;
    }
}

static uint8_t rom_read(uint16_t addr)
{
    if (!gb.cart_rom || gb.cart_rom_size == 0u)
        return 0xFFu;

    if (gb.mbc_type == GB_MBC_NONE || gb.mbc_type == GB_MBC_UNKNOWN)
    {
        size_t index = (size_t)addr;
        if (index >= gb.cart_rom_size)
            return 0xFFu;
        return gb.cart_rom[index];
    }

    if (gb.mbc_type == GB_MBC_MBC1)
    {
        uint8_t bank;
        size_t offset;

        if (addr < 0x4000u)
        {
            bank = 0u;
            if (gb.banking_mode == 1u)
                bank = (uint8_t)(gb.rom_bank_high2 & 0x03u);

            if (gb.rom_bank_count != 0u)
                bank = (uint8_t)(bank % gb.rom_bank_count);

            offset = ((size_t)bank * 0x4000u) + (size_t)addr;
        }
        else
        {
            uint8_t low = (uint8_t)(gb.rom_bank_low5 & 0x1Fu);
            uint8_t high = gb.banking_mode == 0u ? (uint8_t)(gb.rom_bank_high2 & 0x03u) : 0u;

            bank = (uint8_t)(low | (uint8_t)(high << 5));

            if ((bank & 0x1Fu) == 0u)
                bank |= 0x01u;

            if (gb.rom_bank_count != 0u)
                bank = (uint8_t)(bank % gb.rom_bank_count);

            offset = ((size_t)bank * 0x4000u) + (size_t)(addr - 0x4000u);
        }

        if (offset >= gb.cart_rom_size)
            return 0xFFu;

        return gb.cart_rom[offset];
    }

    size_t index = (size_t)addr;
    if (index >= gb.cart_rom_size)
        return 0xFFu;
    return gb.cart_rom[index];
}

static uint8_t ram_read(uint16_t addr)
{
    if (!gb.cart_ram || gb.cart_ram_size == 0u)
        return 0xFFu;

    if (gb.mbc_type == GB_MBC_MBC1)
    {
        if (!gb.ram_enabled)
            return 0xFFu;

        uint8_t bank = 0u;
        if (gb.banking_mode == 1u)
            bank = (uint8_t)(gb.ram_bank & 0x03u);

        size_t offset = ((size_t)bank * 0x2000u) + (size_t)(addr - 0xA000u);
        if (offset >= gb.cart_ram_size)
            return 0xFFu;

        return gb.cart_ram[offset];
    }
    else
    {
        size_t offset = (size_t)(addr - 0xA000u);
        if (offset >= gb.cart_ram_size)
            return 0xFFu;
        return gb.cart_ram[offset];
    }
}

static void ram_write(uint16_t addr, uint8_t value)
{
    if (!gb.cart_ram || gb.cart_ram_size == 0u)
        return;

    if (gb.mbc_type == GB_MBC_MBC1)
    {
        if (!gb.ram_enabled)
            return;

        uint8_t bank = 0u;
        if (gb.banking_mode == 1u)
            bank = (uint8_t)(gb.ram_bank & 0x03u);

        size_t offset = ((size_t)bank * 0x2000u) + (size_t)(addr - 0xA000u);
        if (offset >= gb.cart_ram_size)
            return;

        gb.cart_ram[offset] = value;
    }
    else
    {
        size_t offset = (size_t)(addr - 0xA000u);
        if (offset >= gb.cart_ram_size)
            return;

        gb.cart_ram[offset] = value;
    }
}

static void mbc_handle_rom_write(uint16_t addr, uint8_t value)
{
    if (gb.mbc_type == GB_MBC_NONE || gb.mbc_type == GB_MBC_UNKNOWN)
        return;

    if (gb.mbc_type == GB_MBC_MBC1)
    {
        if (addr <= 0x1FFFu)
        {
            gb.ram_enabled = (uint8_t)(((value & 0x0Fu) == 0x0Au) ? 1u : 0u);
        }
        else if (addr <= 0x3FFFu)
        {
            gb.rom_bank_low5 = (uint8_t)(value & 0x1Fu);
            if ((gb.rom_bank_low5 & 0x1Fu) == 0u)
                gb.rom_bank_low5 = 1u;
        }
        else if (addr <= 0x5FFFu)
        {
            uint8_t v = (uint8_t)(value & 0x03u);
            gb.rom_bank_high2 = v;
            gb.ram_bank = v;
        }
        else if (addr <= 0x7FFFu)
        {
            gb.banking_mode = (uint8_t)(value & 0x01u);
        }
    }
}

static void dma_transfer(uint8_t value)
{
    uint16_t source = (uint16_t)((uint16_t)value << 8);

    for (uint16_t i = 0; i < 0xA0u; ++i)
        gb.mem[(uint16_t)(0xFE00u + i)] = mem_read((uint16_t)(source + i));
}

void memory_init(void)
{
    memset(&gb, 0, sizeof(GB_MEMORY));
    gb.boot_rom_enabled = 1u;
    gb.mbc_type = GB_MBC_NONE;
    gb.rom_bank_count = 2u;
    gb.rom_bank_low5 = 1u;
    gb.ram_enabled = 0u;
}

void memory_reset(void)
{
    memset(gb.mem, 0, sizeof(gb.mem));
    gb.boot_rom_enabled = 1u;

    gb.rom_bank_low5 = 1u;
    gb.rom_bank_high2 = 0u;
    gb.ram_bank = 0u;
    gb.banking_mode = 0u;

    if (gb.mbc_type == GB_MBC_NONE)
        gb.ram_enabled = 1u;
    else
        gb.ram_enabled = 0u;
}

uint8_t mem_read(uint16_t addr)
{
    if (gb.boot_rom_enabled && addr < 0x0100u)
        return gb.boot_rom[addr];

    if (addr < 0x8000u)
        return rom_read(addr);

    if (addr >= 0xA000u && addr <= 0xBFFFu)
        return ram_read(addr);

    if (addr >= 0xE000u && addr <= 0xFDFFu)
        return gb.mem[addr];

    if (addr >= 0xFEA0u && addr <= 0xFEFFu)
        return 0xFFu;

    return gb.mem[addr];
}

void mem_write(uint16_t addr, uint8_t value)
{
    if (addr < 0x8000u)
    {
        mbc_handle_rom_write(addr, value);
        return;
    }

    if (addr >= 0xA000u && addr <= 0xBFFFu)
    {
        ram_write(addr, value);
        return;
    }

    if (addr >= 0xE000u && addr <= 0xFDFFu)
    {
        gb.mem[addr] = value;
        gb.mem[(uint16_t)(addr - 0x2000u)] = value;
        return;
    }

    if (addr >= 0xFEA0u && addr <= 0xFEFFu)
        return;

    if (addr == 0xFF04u)
    {
        gb.mem[0xFF04u] = 0u;
        return;
    }

    if (addr == 0xFF44u)
    {
        gb.mem[0xFF44u] = 0u;
        return;
    }

    if (addr == 0xFF46u)
    {
        gb.mem[0xFF46u] = value;
        dma_transfer(value);
        return;
    }

    if (addr == 0xFF50u)
    {
        gb.mem[0xFF50u] = value;
        if ((value & 0x01u) != 0u)
            gb.boot_rom_enabled = 0u;
        return;
    }

    gb.mem[addr] = value;
}

void load_boot_rom(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp)
        return;

    fread(gb.boot_rom, 1u, 0x100u, fp);
    fclose(fp);
}

void load_cartridge(const char *path)
{
    FILE *fp = fopen(path, "rb");
    if (!fp)
        return;

    fseek(fp, 0, SEEK_END);
    gb.cart_rom_size = (size_t)ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (gb.cart_rom)
    {
        free(gb.cart_rom);
        gb.cart_rom = NULL;
        gb.cart_rom_size = 0u;
    }

    if (gb.cart_ram)
    {
        free(gb.cart_ram);
        gb.cart_ram = NULL;
        gb.cart_ram_size = 0u;
    }

    gb.cart_rom = (uint8_t *)malloc(gb.cart_rom_size);
    if (!gb.cart_rom)
    {
        fclose(fp);
        gb.cart_rom_size = 0u;
        return;
    }

    fread(gb.cart_rom, 1u, gb.cart_rom_size, fp);
    fclose(fp);

    if (gb.cart_rom_size >= 0x150u)
    {
        uint8_t type = gb.cart_rom[0x147u];
        uint8_t rom_size_code = gb.cart_rom[0x148u];
        uint8_t ram_size_code = gb.cart_rom[0x149u];

        gb.cart_type = type;
        gb.mbc_type = detect_mbc_type(type);
        gb.rom_bank_count = rom_bank_count_from_header(rom_size_code);
        gb.ram_bank_count = ram_banks_from_header(ram_size_code);
    }
    else
    {
        gb.cart_type = 0u;
        gb.mbc_type = GB_MBC_NONE;
        gb.rom_bank_count = 2u;
        gb.ram_bank_count = 0u;
    }

    if (gb.ram_bank_count > 0u)
    {
        size_t banks = gb.ram_bank_count;
        gb.cart_ram_size = banks * 0x2000u;

        gb.cart_ram = (uint8_t *)malloc(gb.cart_ram_size);
        if (gb.cart_ram)
            memset(gb.cart_ram, 0, gb.cart_ram_size);
        else
        {
            gb.cart_ram_size = 0u;
            gb.ram_bank_count = 0u;
        }
    }

    gb.rom_bank_low5 = 1u;
    gb.rom_bank_high2 = 0u;
    gb.ram_bank = 0u;
    gb.banking_mode = 0u;

    if (gb.mbc_type == GB_MBC_NONE)
        gb.ram_enabled = 1u;
    else
        gb.ram_enabled = 0u;

    if (gb.cart_rom_size >= 0x8000u)
        memcpy(gb.mem, gb.cart_rom, 0x8000u);
    else
        memcpy(gb.mem, gb.cart_rom, gb.cart_rom_size);
}