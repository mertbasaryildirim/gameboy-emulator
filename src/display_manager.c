#include <stdio.h>
#include <SDL2/SDL.h>

#include "display_manager.h"
#include "gb_ppu.h"
#include "gb_joypad.h"

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

    {
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

static void dm_handle_key_event(const SDL_KeyboardEvent *key, uint8_t pressed)
{
    SDL_Scancode sc = key->keysym.scancode;

    switch (sc)
    {
    case SDL_SCANCODE_RIGHT:
        gb_joypad_set_button(GB_JOY_RIGHT, pressed);
        break;
    case SDL_SCANCODE_LEFT:
        gb_joypad_set_button(GB_JOY_LEFT, pressed);
        break;
    case SDL_SCANCODE_UP:
        gb_joypad_set_button(GB_JOY_UP, pressed);
        break;
    case SDL_SCANCODE_DOWN:
        gb_joypad_set_button(GB_JOY_DOWN, pressed);
        break;
    case SDL_SCANCODE_Z:
        gb_joypad_set_button(GB_JOY_A, pressed);
        break;
    case SDL_SCANCODE_X:
        gb_joypad_set_button(GB_JOY_B, pressed);
        break;
    case SDL_SCANCODE_RSHIFT:
        gb_joypad_set_button(GB_JOY_SELECT, pressed);
        break;
    case SDL_SCANCODE_RETURN:
        gb_joypad_set_button(GB_JOY_START, pressed);
        break;
    default:
        break;
    }
}

int dm_handle_events(void)
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        if (ev.type == SDL_QUIT)
            return 0;

        if (ev.type == SDL_KEYDOWN)
        {
            if (ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                return 0;

            dm_handle_key_event(&ev.key, 1u);
        }
        else if (ev.type == SDL_KEYUP)
        {
            dm_handle_key_event(&ev.key, 0u);
        }
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