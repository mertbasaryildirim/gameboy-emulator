#include <string.h>
#include <stdint.h>

#include "gb_apu.h"
#include "gb_memory.h"
#include "memory_map.h"
#include "audio_manager.h"

#define GB_CPU_CLOCK_HZ 4194304.0
#define APU_SAMPLE_BUFFER_SIZE 1024
#define FRAME_SEQ_PERIOD_CYCLES 8192u

typedef struct
{
    uint8_t enabled;
    uint8_t dac_enabled;

    uint8_t length;
    uint8_t length_enable;
    uint8_t duty;
    double phase;
    double phase_step;

    uint16_t freq_reg;

    uint8_t initial_volume;
    uint8_t volume;
    uint8_t envelope_period;
    uint8_t envelope_counter;
    uint8_t envelope_direction;

    uint8_t sweep_time;
    uint8_t sweep_shift;
    uint8_t sweep_direction;
    uint8_t sweep_counter;
    uint8_t sweep_enabled;
} APU_SQUARE;

typedef struct
{
    uint8_t enabled;
    uint8_t dac_enabled;

    uint8_t length;
    uint8_t length_enable;

    uint16_t freq_reg;
    double phase;
    double phase_step;

    uint8_t volume_code;
} APU_WAVE;

typedef struct
{
    uint8_t enabled;
    uint8_t dac_enabled;

    uint8_t length;
    uint8_t length_enable;

    uint8_t initial_volume;
    uint8_t volume;
    uint8_t envelope_period;
    uint8_t envelope_counter;
    uint8_t envelope_direction;

    uint8_t clock_shift;
    uint8_t width_mode;
    uint8_t divisor_code;

    double phase;
    double phase_step;

    uint16_t lfsr;
} APU_NOISE;

typedef struct
{
    uint8_t enabled;

    uint32_t frame_counter_cycles;
    uint8_t frame_step;

    double cycles_per_sample;
    double sample_timer;
    int sample_rate;

    int16_t sample_buffer[APU_SAMPLE_BUFFER_SIZE];
    size_t sample_buffer_pos;

    APU_SQUARE ch1;
    APU_SQUARE ch2;
    APU_WAVE ch3;
    APU_NOISE ch4;
} GB_APU_STATE;

static GB_APU_STATE apu;

static const uint8_t duty_patterns[4] = {
    0x01u, /* 12.5% */
    0x03u, /* 25% */
    0x0Fu, /* 50% */
    0xFCu  /* 75% */
};

static void apu_update_square_freq(APU_SQUARE *ch)
{
    if (ch->freq_reg > 2047u)
        ch->freq_reg = 2047u;

    {
        uint16_t val = (uint16_t)(2048u - ch->freq_reg);
        double freq;
        if (val == 0u)
            val = 1u;
        freq = 131072.0 / (double)val;
        if (apu.sample_rate <= 0)
            ch->phase_step = 0.0;
        else
            ch->phase_step = (freq * 8.0) / (double)apu.sample_rate;
    }
}

static void apu_update_wave_freq(void)
{
    if (apu.ch3.freq_reg > 2047u)
        apu.ch3.freq_reg = 2047u;

    {
        uint16_t val = (uint16_t)(2048u - apu.ch3.freq_reg);
        double freq;
        if (val == 0u)
            val = 1u;
        freq = 65536.0 / (double)val;
        if (apu.sample_rate <= 0)
            apu.ch3.phase_step = 0.0;
        else
            apu.ch3.phase_step = (freq * 32.0) / (double)apu.sample_rate;
    }
}

static double apu_noise_freq_from_reg(uint8_t reg)
{
    uint8_t shift = (uint8_t)((reg >> 4) & 0x0Fu);
    uint8_t div_code = (uint8_t)(reg & 0x07u);
    double divisor;

    switch (div_code)
    {
    case 0:
        divisor = 4.0;
        break;
    case 1:
        divisor = 8.0;
        break;
    case 2:
        divisor = 16.0;
        break;
    case 3:
        divisor = 24.0;
        break;
    case 4:
        divisor = 32.0;
        break;
    case 5:
        divisor = 40.0;
        break;
    case 6:
        divisor = 48.0;
        break;
    case 7:
        divisor = 56.0;
        break;
    default:
        divisor = 8.0;
        break;
    }

    {
        double freq = 524288.0 / divisor;
        freq /= (double)(1u << (shift + 1u));
        return freq;
    }
}

static void apu_update_noise_freq(void)
{
    double freq = apu_noise_freq_from_reg((uint8_t)((apu.ch4.clock_shift << 4) |
                                                    (apu.ch4.width_mode ? 0x08u : 0u) |
                                                    (apu.ch4.divisor_code & 0x07u)));
    if (apu.sample_rate <= 0)
        apu.ch4.phase_step = 0.0;
    else
        apu.ch4.phase_step = freq / (double)apu.sample_rate;
}

static void apu_length_clock(void)
{
    if (apu.ch1.length_enable && apu.ch1.length > 0u)
    {
        apu.ch1.length--;
        if (apu.ch1.length == 0u)
            apu.ch1.enabled = 0u;
    }

    if (apu.ch2.length_enable && apu.ch2.length > 0u)
    {
        apu.ch2.length--;
        if (apu.ch2.length == 0u)
            apu.ch2.enabled = 0u;
    }

    if (apu.ch3.length_enable && apu.ch3.length > 0u)
    {
        apu.ch3.length--;
        if (apu.ch3.length == 0u)
            apu.ch3.enabled = 0u;
    }

    if (apu.ch4.length_enable && apu.ch4.length > 0u)
    {
        apu.ch4.length--;
        if (apu.ch4.length == 0u)
            apu.ch4.enabled = 0u;
    }
}

static void apu_sweep_clock(void)
{
    APU_SQUARE *ch = &apu.ch1;

    if (!ch->enabled)
        return;

    if (ch->sweep_time == 0u && ch->sweep_shift == 0u)
        return;

    if (ch->sweep_counter > 0u)
        ch->sweep_counter--;

    if (ch->sweep_counter == 0u)
    {
        uint16_t freq = ch->freq_reg;
        uint16_t delta;

        ch->sweep_counter = ch->sweep_time ? ch->sweep_time : 8u;

        if (ch->sweep_shift == 0u)
            return;

        delta = (uint16_t)(freq >> ch->sweep_shift);
        if (ch->sweep_direction)
            freq = (uint16_t)(freq - delta);
        else
            freq = (uint16_t)(freq + delta);

        if (freq > 2047u)
        {
            ch->enabled = 0u;
        }
        else
        {
            ch->freq_reg = freq;
            apu_update_square_freq(ch);
        }
    }
}

static void apu_envelope_clock(void)
{
    APU_SQUARE *sqr[2] = {&apu.ch1, &apu.ch2};
    int i;

    for (i = 0; i < 2; ++i)
    {
        APU_SQUARE *ch = sqr[i];
        if (!ch->enabled)
            continue;

        if (ch->envelope_period == 0u)
            continue;

        if (ch->envelope_counter > 0u)
            ch->envelope_counter--;

        if (ch->envelope_counter == 0u)
        {
            ch->envelope_counter = ch->envelope_period;

            if (ch->envelope_direction)
            {
                if (ch->volume < 15u)
                    ch->volume++;
            }
            else
            {
                if (ch->volume > 0u)
                    ch->volume--;
            }
        }
    }

    if (apu.ch4.enabled && apu.ch4.envelope_period != 0u)
    {
        if (apu.ch4.envelope_counter > 0u)
            apu.ch4.envelope_counter--;

        if (apu.ch4.envelope_counter == 0u)
        {
            apu.ch4.envelope_counter = apu.ch4.envelope_period;

            if (apu.ch4.envelope_direction)
            {
                if (apu.ch4.volume < 15u)
                    apu.ch4.volume++;
            }
            else
            {
                if (apu.ch4.volume > 0u)
                    apu.ch4.volume--;
            }
        }
    }
}

static int apu_square_sample(APU_SQUARE *ch)
{
    int amp = 0;

    if (!ch->enabled || !ch->dac_enabled)
        return 0;

    ch->phase += ch->phase_step;
    if (ch->phase >= 8.0)
        ch->phase -= 8.0;

    {
        int idx = (int)ch->phase & 7;
        uint8_t pattern = duty_patterns[ch->duty & 0x03u];
        uint8_t bit = (pattern >> (7 - idx)) & 0x01u;

        if (bit)
            amp = (int)ch->volume;
        else
            amp = -(int)ch->volume;
    }

    return amp;
}

static int apu_wave_sample(void)
{
    int amp = 0;

    if (!apu.ch3.enabled || !apu.ch3.dac_enabled)
        return 0;

    apu.ch3.phase += apu.ch3.phase_step;
    if (apu.ch3.phase >= 32.0)
        apu.ch3.phase -= 32.0;

    {
        int idx = (int)apu.ch3.phase;
        uint8_t byte = gb.mem[0xFF30u + (idx >> 1)];
        uint8_t sample4 = (uint8_t)((idx & 1) ? (byte & 0x0Fu) : (byte >> 4));
        int s = (int)sample4 - 8;

        switch (apu.ch3.volume_code & 0x03u)
        {
        case 0:
            s = 0;
            break;
        case 1:
            break;
        case 2:
            s >>= 1;
            break;
        case 3:
            s >>= 2;
            break;
        }

        amp = s;
    }

    return amp;
}

static int apu_noise_sample(void)
{
    int amp = 0;

    if (!apu.ch4.enabled || !apu.ch4.dac_enabled)
        return 0;

    apu.ch4.phase += apu.ch4.phase_step;
    while (apu.ch4.phase >= 1.0)
    {
        uint16_t xor_bit = (uint16_t)((apu.ch4.lfsr & 1u) ^ ((apu.ch4.lfsr >> 1) & 1u));
        apu.ch4.phase -= 1.0;
        apu.ch4.lfsr >>= 1;
        apu.ch4.lfsr |= (uint16_t)(xor_bit << 14);

        if (apu.ch4.width_mode)
        {
            apu.ch4.lfsr &= (uint16_t)~(1u << 6);
            apu.ch4.lfsr |= (uint16_t)(xor_bit << 6);
        }
    }

    if ((apu.ch4.lfsr & 1u) == 0u)
        amp = (int)apu.ch4.volume;
    else
        amp = -(int)apu.ch4.volume;

    return amp;
}

static int16_t apu_mix_sample(void)
{
    uint8_t nr50 = gb.mem[0xFF24u];
    uint8_t nr51 = gb.mem[0xFF25u];

    int ch1 = apu_square_sample(&apu.ch1);
    int ch2 = apu_square_sample(&apu.ch2);
    int ch3 = apu_wave_sample();
    int ch4 = apu_noise_sample();

    int left = 0;
    int right = 0;

    if (nr51 & 0x10u)
        left += ch1;
    if (nr51 & 0x01u)
        right += ch1;

    if (nr51 & 0x20u)
        left += ch2;
    if (nr51 & 0x02u)
        right += ch2;

    if (nr51 & 0x40u)
        left += ch3;
    if (nr51 & 0x04u)
        right += ch3;

    if (nr51 & 0x80u)
        left += ch4;
    if (nr51 & 0x08u)
        right += ch4;

    {
        int left_vol = (nr50 >> 4) & 0x07u;
        int right_vol = nr50 & 0x07u;

        int mixed = 0;

        mixed += left * (left_vol + 1);
        mixed += right * (right_vol + 1);
        mixed /= 2;

        {
            int out = mixed * 32;
            if (out > 32767)
                out = 32767;
            else if (out < -32768)
                out = -32768;
            return (int16_t)out;
        }
    }
}

void gb_apu_init(int sample_rate)
{
    memset(&apu, 0, sizeof(apu));
    apu.sample_rate = sample_rate;
    apu.cycles_per_sample = GB_CPU_CLOCK_HZ / (double)sample_rate;
    apu.sample_timer = 0.0;
    apu.sample_buffer_pos = 0;

    apu.enabled = 0u;

    apu.ch4.lfsr = 0x7FFFu;
}

void gb_apu_reset(void)
{
    int sample_rate = apu.sample_rate;
    gb_apu_init(sample_rate);
}

static void apu_trigger_square(APU_SQUARE *ch, int is_ch1)
{
    if (!ch->dac_enabled)
    {
        ch->enabled = 0u;
        return;
    }

    ch->enabled = 1u;

    if (ch->length == 0u)
        ch->length = 64u;

    ch->volume = ch->initial_volume;
    ch->envelope_counter = ch->envelope_period ? ch->envelope_period : 8u;

    apu_update_square_freq(ch);

    if (is_ch1)
    {
        ch->sweep_counter = ch->sweep_time ? ch->sweep_time : 8u;
        ch->sweep_enabled = (uint8_t)((ch->sweep_time != 0u) || (ch->sweep_shift != 0u));
    }
}

static void apu_trigger_wave(void)
{
    if (!apu.ch3.dac_enabled)
    {
        apu.ch3.enabled = 0u;
        return;
    }

    apu.ch3.enabled = 1u;

    if (apu.ch3.length == 0u)
        apu.ch3.length = 256u;

    apu.ch3.phase = 0.0;
    apu_update_wave_freq();
}

static void apu_trigger_noise(void)
{
    if (!apu.ch4.dac_enabled)
    {
        apu.ch4.enabled = 0u;
        return;
    }

    apu.ch4.enabled = 1u;

    if (apu.ch4.length == 0u)
        apu.ch4.length = 64u;

    apu.ch4.volume = apu.ch4.initial_volume;
    apu.ch4.envelope_counter = apu.ch4.envelope_period ? apu.ch4.envelope_period : 8u;
    apu.ch4.lfsr = 0x7FFFu;
    apu_update_noise_freq();
}

uint8_t gb_apu_read(uint16_t addr)
{
    if (addr == 0xFF26u)
    {
        uint8_t v = gb.mem[0xFF26u];
        uint8_t status = 0u;

        if (apu.ch1.enabled)
            status |= 0x01u;
        if (apu.ch2.enabled)
            status |= 0x02u;
        if (apu.ch3.enabled)
            status |= 0x04u;
        if (apu.ch4.enabled)
            status |= 0x08u;

        v &= 0xF0u;
        v |= status;

        if (apu.enabled)
            v |= 0x80u;
        else
            v &= (uint8_t)~0x80u;

        return v;
    }

    return gb.mem[addr];
}

void gb_apu_write(uint16_t addr, uint8_t value)
{
    gb.mem[addr] = value;

    if (addr >= 0xFF30u && addr <= 0xFF3Fu)
        return;

    if (addr < 0xFF10u || addr > 0xFF26u)
        return;

    switch (addr)
    {
    case 0xFF10u:
    {
        uint8_t sweep_time = (uint8_t)((value >> 4) & 0x07u);
        uint8_t sweep_dir = (uint8_t)((value >> 3) & 0x01u);
        uint8_t sweep_shift = (uint8_t)(value & 0x07u);

        apu.ch1.sweep_time = sweep_time;
        apu.ch1.sweep_direction = sweep_dir;
        apu.ch1.sweep_shift = sweep_shift;
    }
    break;

    case 0xFF11u:
        apu.ch1.duty = (uint8_t)((value >> 6) & 0x03u);
        apu.ch1.length = (uint8_t)(64u - (value & 0x3Fu));
        break;

    case 0xFF12u:
        apu.ch1.initial_volume = (uint8_t)((value >> 4) & 0x0Fu);
        apu.ch1.envelope_direction = (uint8_t)((value >> 3) & 0x01u);
        apu.ch1.envelope_period = (uint8_t)(value & 0x07u);
        apu.ch1.dac_enabled = (uint8_t)((value & 0xF8u) != 0u);
        if (!apu.ch1.dac_enabled)
            apu.ch1.enabled = 0u;
        break;

    case 0xFF13u:
        apu.ch1.freq_reg = (uint16_t)((apu.ch1.freq_reg & 0x0700u) | value);
        apu_update_square_freq(&apu.ch1);
        break;

    case 0xFF14u:
        apu.ch1.freq_reg = (uint16_t)((apu.ch1.freq_reg & 0x00FFu) | ((uint16_t)(value & 0x07u) << 8));
        apu.ch1.length_enable = (uint8_t)((value & 0x40u) != 0u);
        apu_update_square_freq(&apu.ch1);
        if (value & 0x80u)
            apu_trigger_square(&apu.ch1, 1);
        break;

    case 0xFF16u:
        apu.ch2.duty = (uint8_t)((value >> 6) & 0x03u);
        apu.ch2.length = (uint8_t)(64u - (value & 0x3Fu));
        break;

    case 0xFF17u:
        apu.ch2.initial_volume = (uint8_t)((value >> 4) & 0x0Fu);
        apu.ch2.envelope_direction = (uint8_t)((value >> 3) & 0x01u);
        apu.ch2.envelope_period = (uint8_t)(value & 0x07u);
        apu.ch2.dac_enabled = (uint8_t)((value & 0xF8u) != 0u);
        if (!apu.ch2.dac_enabled)
            apu.ch2.enabled = 0u;
        break;

    case 0xFF18u:
        apu.ch2.freq_reg = (uint16_t)((apu.ch2.freq_reg & 0x0700u) | value);
        apu_update_square_freq(&apu.ch2);
        break;

    case 0xFF19u:
        apu.ch2.freq_reg = (uint16_t)((apu.ch2.freq_reg & 0x00FFu) | ((uint16_t)(value & 0x07u) << 8));
        apu.ch2.length_enable = (uint8_t)((value & 0x40u) != 0u);
        apu_update_square_freq(&apu.ch2);
        if (value & 0x80u)
            apu_trigger_square(&apu.ch2, 0);
        break;

    case 0xFF1Au:
        apu.ch3.dac_enabled = (uint8_t)((value & 0x80u) != 0u);
        if (!apu.ch3.dac_enabled)
            apu.ch3.enabled = 0u;
        break;

    case 0xFF1Bu:
        apu.ch3.length = (uint8_t)(256u - value);
        break;

    case 0xFF1Cu:
        apu.ch3.volume_code = (uint8_t)((value >> 5) & 0x03u);
        break;

    case 0xFF1Du:
        apu.ch3.freq_reg = (uint16_t)((apu.ch3.freq_reg & 0x0700u) | value);
        apu_update_wave_freq();
        break;

    case 0xFF1Eu:
        apu.ch3.freq_reg = (uint16_t)((apu.ch3.freq_reg & 0x00FFu) | ((uint16_t)(value & 0x07u) << 8));
        apu.ch3.length_enable = (uint8_t)((value & 0x40u) != 0u);
        apu_update_wave_freq();
        if (value & 0x80u)
            apu_trigger_wave();
        break;

    case 0xFF20u:
        apu.ch4.length = (uint8_t)(64u - (value & 0x3Fu));
        break;

    case 0xFF21u:
        apu.ch4.initial_volume = (uint8_t)((value >> 4) & 0x0Fu);
        apu.ch4.envelope_direction = (uint8_t)((value >> 3) & 0x01u);
        apu.ch4.envelope_period = (uint8_t)(value & 0x07u);
        apu.ch4.dac_enabled = (uint8_t)((value & 0xF8u) != 0u);
        if (!apu.ch4.dac_enabled)
            apu.ch4.enabled = 0u;
        break;

    case 0xFF22u:
        apu.ch4.clock_shift = (uint8_t)((value >> 4) & 0x0Fu);
        apu.ch4.width_mode = (uint8_t)((value >> 3) & 0x01u);
        apu.ch4.divisor_code = (uint8_t)(value & 0x07u);
        apu_update_noise_freq();
        break;

    case 0xFF23u:
        apu.ch4.length_enable = (uint8_t)((value & 0x40u) != 0u);
        if (value & 0x80u)
            apu_trigger_noise();
        break;

    case 0xFF24u:
    case 0xFF25u:
        break;

    case 0xFF26u:
        if (value & 0x80u)
        {
            apu.enabled = 1u;
        }
        else
        {
            apu.enabled = 0u;
            apu.ch1.enabled = 0u;
            apu.ch2.enabled = 0u;
            apu.ch3.enabled = 0u;
            apu.ch4.enabled = 0u;
        }
        break;

    default:
        break;
    }
}

void gb_apu_step(uint8_t m_cycles)
{
    uint32_t tcycles = (uint32_t)m_cycles * 4u;

    if (!apu.enabled)
        return;

    apu.frame_counter_cycles += tcycles;
    while (apu.frame_counter_cycles >= FRAME_SEQ_PERIOD_CYCLES)
    {
        apu.frame_counter_cycles -= FRAME_SEQ_PERIOD_CYCLES;
        apu.frame_step = (uint8_t)((apu.frame_step + 1u) & 0x07u);

        switch (apu.frame_step)
        {
        case 0:
        case 2:
        case 4:
        case 6:
            apu_length_clock();
            if (apu.frame_step == 2 || apu.frame_step == 6)
                apu_sweep_clock();
            break;
        case 7:
            apu_envelope_clock();
            break;
        default:
            break;
        }
    }

    apu.sample_timer += (double)tcycles;
    while (apu.sample_timer >= apu.cycles_per_sample)
    {
        apu.sample_timer -= apu.cycles_per_sample;

        {
            int16_t s = apu_mix_sample();
            apu.sample_buffer[apu.sample_buffer_pos++] = s;

            if (apu.sample_buffer_pos >= APU_SAMPLE_BUFFER_SIZE)
            {
                audio_queue_samples(apu.sample_buffer, APU_SAMPLE_BUFFER_SIZE);
                apu.sample_buffer_pos = 0;
            }
        }
    }
}