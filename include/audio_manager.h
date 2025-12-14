#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <stddef.h>
#include <stdint.h>

int audio_init(int sample_rate);
void audio_shutdown(void);
void audio_queue_samples(const int16_t *samples, size_t count);

#endif