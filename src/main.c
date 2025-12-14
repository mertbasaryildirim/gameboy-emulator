#include <stdio.h>

#include "gb_memory.h"
#include "gb_processor.h"
#include "gb_timer.h"
#include "gb_ppu.h"
#include "gb_apu.h"
#include "display_manager.h"
#include "gb_debug.h"
#include "audio_manager.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <boot_rom.bin> <game_rom.gb>\n", argv[0]);
        return 1;
    }

    memory_init();
    load_boot_rom(argv[1]);
    load_cartridge(argv[2]);

    gb_cpu_init();
    gb_timer_init();
    gb_ppu_init();

    {
        int sample_rate = 44100;
        gb_apu_init(sample_rate);

        if (dm_init("Game Boy Emulator", 4) != 0)
            return 1;

        if (audio_init(sample_rate) != 0)
        {
            dm_shutdown();
            return 1;
        }
    }

    gb_debug_init();

    {
        int running = 1;

        while (running)
        {
            if (!dm_handle_events())
            {
                running = 0;
                break;
            }

            gb_ppu_clear_frame_complete();

            while (!gb_ppu.frame_complete && running)
            {
                gb_cpu_step();

                if (!dm_handle_events())
                    running = 0;
            }

            if (!running)
                break;

            dm_present(gb_ppu.framebuffer);
            gb_debug_frame();
        }
    }

    audio_shutdown();
    dm_shutdown();
    return 0;
}