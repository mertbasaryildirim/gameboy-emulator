#include <stdio.h>
#include <SDL2/SDL.h>

#include "audio_manager.h"

static SDL_AudioDeviceID audio_dev = 0;

int audio_init(int sample_rate)
{
    SDL_AudioSpec spec;
    SDL_AudioSpec obtained;

    if (audio_dev != 0)
        return 0;

    SDL_zero(spec);
    spec.freq = sample_rate;
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples = 1024;
    spec.callback = NULL;

    audio_dev = SDL_OpenAudioDevice(NULL, 0, &spec, &obtained, 0);
    if (audio_dev == 0)
    {
        printf("SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_PauseAudioDevice(audio_dev, 0);
    return 0;
}

void audio_shutdown(void)
{
    if (audio_dev != 0)
    {
        SDL_CloseAudioDevice(audio_dev);
        audio_dev = 0;
    }
}

void audio_queue_samples(const int16_t *samples, size_t count)
{
    if (audio_dev == 0)
        return;

    SDL_QueueAudio(audio_dev, samples, (Uint32)(count * sizeof(int16_t)));
}