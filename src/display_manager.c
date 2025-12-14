#include <stdio.h>
#include <SDL2/SDL.h>

#include "display_manager.h"
#include "gb_ppu.h"

static SDL_Window *dm_window = NULL;
static SDL_Renderer *dm_renderer = NULL;
static SDL_Texture *dm_texture = NULL;

int dm_init(const char *title, int scale)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    int win_w = GB_LCD_WIDTH * scale;
    int win_h = GB_LCD_HEIGHT * scale;

    dm_window = SDL_CreateWindow(title,
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 win_w,
                                 win_h,
                                 SDL_WINDOW_SHOWN);
    if (!dm_window)
    {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    dm_renderer = SDL_CreateRenderer(dm_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!dm_renderer)
    {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(dm_window);
        dm_window = NULL;
        SDL_Quit();
        return 1;
    }

    dm_texture = SDL_CreateTexture(dm_renderer,
                                   SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   GB_LCD_WIDTH,
                                   GB_LCD_HEIGHT);
    if (!dm_texture)
    {
        printf("SDL_CreateTexture failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(dm_renderer);
        dm_renderer = NULL;
        SDL_DestroyWindow(dm_window);
        dm_window = NULL;
        SDL_Quit();
        return 1;
    }

    return 0;
}

void dm_shutdown(void)
{
    if (dm_texture)
    {
        SDL_DestroyTexture(dm_texture);
        dm_texture = NULL;
    }
    if (dm_renderer)
    {
        SDL_DestroyRenderer(dm_renderer);
        dm_renderer = NULL;
    }
    if (dm_window)
    {
        SDL_DestroyWindow(dm_window);
        dm_window = NULL;
    }
    SDL_Quit();
}

int dm_handle_events(void)
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        if (ev.type == SDL_QUIT)
            return 0;
    }
    return 1;
}

void dm_present(const uint32_t *framebuffer)
{
    if (!dm_renderer || !dm_texture)
        return;

    SDL_UpdateTexture(dm_texture, NULL, framebuffer, GB_LCD_WIDTH * (int)sizeof(uint32_t));
    SDL_RenderClear(dm_renderer);
    SDL_RenderCopy(dm_renderer, dm_texture, NULL, NULL);
    SDL_RenderPresent(dm_renderer);
}