#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

#include "gb_memory.h"

#define IO_PI gb.mem[0xFF00]
#define IO_SB gb.mem[0xFF01]
#define IO_SC gb.mem[0xFF02]

#define IO_DIV gb.mem[0xFF04]
#define IO_TIMA gb.mem[0xFF05]
#define IO_TMA gb.mem[0xFF06]
#define IO_TAC gb.mem[0xFF07]

#define IO_IF gb.mem[0xFF0F]

#define IO_LCDC gb.mem[0xFF40]
#define IO_STAT gb.mem[0xFF41]
#define IO_SCY gb.mem[0xFF42]
#define IO_SCX gb.mem[0xFF43]
#define IO_LY gb.mem[0xFF44]
#define IO_LCY gb.mem[0xFF45]
#define IO_DMA gb.mem[0xFF46]
#define IO_BGP gb.mem[0xFF47]
#define IO_OBP0 gb.mem[0xFF48]
#define IO_OBP1 gb.mem[0xFF49]
#define IO_WY gb.mem[0xFF4A]
#define IO_WX gb.mem[0xFF48]

#define IO_IE gb.mem[0xFFFF]

#endif