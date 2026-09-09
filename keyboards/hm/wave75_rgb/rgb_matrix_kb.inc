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

RGB_MATRIX_EFFECT(HEART_AEMEATH)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

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

#endif