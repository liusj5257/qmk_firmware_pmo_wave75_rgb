/* Copyright 2026 RDMCTMZT
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#define HEART_PINK_R 255
#define HEART_PINK_G 0
#define HEART_PINK_B 40
#define HEART_BLUE_R 0
#define HEART_BLUE_G 255
#define HEART_BLUE_B 255

// Lower the number for a denser effect / wider keyboard.
#ifndef RGB_DIGITAL_RAIN_DROPS
#    define RGB_DIGITAL_RAIN_DROPS 24
#endif

RGB_MATRIX_EFFECT(HEART_AEMEATH)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#    ifdef RGB_MATRIX_FRAMEBUFFER_EFFECTS

// LED indices that form the heart region: digital rain is confined to these.
#        define HEART_LED_COUNT 20
static const uint8_t HEART_LEDS[HEART_LED_COUNT] = {
    23, 24, 26, 27,
    37, 38, 39, 40, 41, 42,
    53, 54, 55, 56, 57,
    67, 68, 69, 70,
    77,
};

static bool heart_aemeath_led_in_region(uint8_t led) {
    for (uint8_t i = 0; i < HEART_LED_COUNT; i++) {
        if (HEART_LEDS[i] == led) {
            return true;
        }
    }
    return false;
}

// Digital rain restricted to the heart region; every other LED stays off.
static bool HEART_AEMEATH(effect_params_t *params) {
    // algorithm ported from https://github.com/tremby/Kaleidoscope-LEDEffect-DigitalRain
    const uint8_t max_intensity = rgb_matrix_config.hsv.v;
    static uint8_t drop  = 0;
    static uint8_t decay = 0;

    if (params->init) {
        rgb_matrix_set_color_all(0, 0, 0);
        memset(g_rgb_frame_buffer, 0, sizeof(g_rgb_frame_buffer));
        drop  = 0;
        decay = 0;
    }

    if (max_intensity == 0) {
        // RGB matrix off / zero brightness: nothing to draw
        return false;
    }

    const uint8_t drop_ticks           = 28;
    const uint8_t pure_green_intensity = (((uint16_t)max_intensity) * 3) >> 2;
    const uint8_t max_brightness_boost = (((uint16_t)max_intensity) * 3) >> 2;
    const uint8_t decay_ticks          = 0xff / max_intensity;

    decay++;
    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
        for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
            if (row == 0 && drop == 0 && rand() < RAND_MAX / RGB_DIGITAL_RAIN_DROPS) {
                // top row, pixels have just fallen and we're
                // making a new rain drop in this column
                g_rgb_frame_buffer[row][col] = max_intensity;
            } else if (g_rgb_frame_buffer[row][col] > 0 && g_rgb_frame_buffer[row][col] < max_intensity) {
                // neither fully bright nor dark, decay it
                if (decay == decay_ticks) {
                    g_rgb_frame_buffer[row][col]--;
                }
            }

            // set the pixel colour, but only for LEDs inside the heart region
            uint8_t led[LED_HITS_TO_REMEMBER];
            uint8_t led_count = rgb_matrix_map_row_column_to_led(row, col, led);
            if (led_count > 0) {
                const uint8_t rain = g_rgb_frame_buffer[row][col];
                for (uint8_t index = 0; index < led_count; index++) {
                    if (!heart_aemeath_led_in_region(led[index])) {
                        // outside the heart region: keep the LED off
                        rgb_matrix_set_color(led[index], 0, 0, 0);
                        continue;
                    }

                    if (rain > pure_green_intensity) {
                        const uint8_t boost = (uint8_t)((uint16_t)max_brightness_boost * (rain - pure_green_intensity) / (max_intensity - pure_green_intensity));
                        rgb_matrix_set_color(led[index], boost, max_intensity, boost);
                    } else if (rain > 0) {
                        const uint8_t green = (uint8_t)((uint16_t)max_intensity * rain / pure_green_intensity);
                        rgb_matrix_set_color(led[index], 0, green, 0);
                    } else {
                        rgb_matrix_set_color(led[index], 0, 0, 0);
                    }
                }
            }
        }
    }

    if (decay == decay_ticks) {
        decay = 0;
    }

    if (++drop > drop_ticks) {
        // reset drop timer
        drop = 0;
        for (uint8_t row = MATRIX_ROWS - 1; row > 0; row--) {
            for (uint8_t col = 0; col < MATRIX_COLS; col++) {
                // if this is on the bottom row and bright allow decay
                if (row == MATRIX_ROWS - 1 && g_rgb_frame_buffer[row][col] == max_intensity) {
                    g_rgb_frame_buffer[row][col]--;
                }
                // check if the pixel above is bright
                if (g_rgb_frame_buffer[row - 1][col] >= max_intensity) { // Note: can be larger than max_intensity if val was recently decreased
                    // allow old bright pixel to decay
                    g_rgb_frame_buffer[row - 1][col] = max_intensity - 1;
                    // make this pixel bright
                    g_rgb_frame_buffer[row][col] = max_intensity;
                }
            }
        }
    }

    return false;
}

#    else // RGB_MATRIX_FRAMEBUFFER_EFFECTS

// Fallback: framebuffer not enabled -> keep the original static gradient heart.
static uint8_t heart_aemeath_mix(uint8_t first, uint8_t second, uint8_t amount) {
    return scale8(first, 255 - amount) + scale8(second, amount);
}

static void heart_aemeath_set_color(uint8_t led, uint8_t gradient) {
    rgb_matrix_set_color(
        led,
        heart_aemeath_mix(HEART_PINK_R, HEART_BLUE_R, gradient),
        heart_aemeath_mix(HEART_PINK_G, HEART_BLUE_G, gradient),
        heart_aemeath_mix(HEART_PINK_B, HEART_BLUE_B, gradient)
    );
}

static bool HEART_AEMEATH(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS_ITER(min, max, params->iter);

    for (uint8_t i = min; i < max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();

        switch (i) {
            case 23:
            case 24:
            case 26:
            case 27:
                heart_aemeath_set_color(i, 0);
                break;
            case 37:
            case 38:
            case 39:
            case 40:
            case 41:
            case 42:
                heart_aemeath_set_color(i, 40);
                break;
            case 53:
            case 54:
            case 55:
            case 56:
            case 57:
                heart_aemeath_set_color(i, 128);
                break;
            case 67:
            case 68:
            case 69:
            case 70:
                heart_aemeath_set_color(i, 216);
                break;
            case 77:
                heart_aemeath_set_color(i, 255);
                break;
        }
    }

    return rgb_matrix_check_finished_leds(max);
}
#    endif // RGB_MATRIX_FRAMEBUFFER_EFFECTS

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
