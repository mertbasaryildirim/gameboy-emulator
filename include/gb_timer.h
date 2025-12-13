#ifndef GB_TIMER_H
#define GB_TIMER_H

#include <stdint.h>

void gb_timer_init(void);
void gb_timer_reset(void);
void gb_timer_step(uint8_t m_cycles);
void gb_timer_div_reset(void);

#endif