#ifndef GB_APU_H
#define GB_APU_H

#include <stdint.h>

void gb_apu_init(int sample_rate);
void gb_apu_reset(void);
void gb_apu_step(uint8_t m_cycles);

uint8_t gb_apu_read(uint16_t addr);
void gb_apu_write(uint16_t addr, uint8_t value);

#endif