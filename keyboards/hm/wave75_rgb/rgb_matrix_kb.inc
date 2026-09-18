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
#define HEART_BLUE_G 0
#define HEART_BLUE_B 255
#define HEART_RED_R 255
#define HEART_RED_G 0
#define HEART_RED_B 0

// 波峰纹波颜色（可直接修改 RGB 数值）
#define HEART_RIPPLE_R 255
#define HEART_RIPPLE_G 0
#define HEART_RIPPLE_B 0

// 波谷纹波颜色（可直接修改 RGB 数值）
#define HEART_VALLEY_RIPPLE_R 0
#define HEART_VALLEY_RIPPLE_G 0
#define HEART_VALLEY_RIPPLE_B 255

// 星尘颜色（可直接修改 RGB 数值）
#define HEART_STARDUST_R 255
#define HEART_STARDUST_G 0
#define HEART_STARDUST_B 255

// 心外 LED 默认背景色（可直接修改 RGB 数值）
#define HEART_BG_R 255
#define HEART_BG_G 0
#define HEART_BG_B 0
#define HEART_BG_BRIGHTNESS 10

// 心跳周期范围（由 VIA 速度控制映射到此区间）
#define HEARTBEAT_PERIOD_SLOW_MS 2400
#define HEARTBEAT_PERIOD_FAST_MS 400
#define RIPPLE_WIDTH 40

// 星尘参数
#define STARDust_COUNT 16
#define STARDust_DECAY_MS 1800
#define STARDust_MAX_BRIGHT 200

RGB_MATRIX_EFFECT(HEART_AEMEATH)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

// LED indices that form the heart region
#    define HEART_LED_COUNT 20
static const uint8_t HEART_LEDS[HEART_LED_COUNT] = {
    23, 24, 26, 27,
    37, 38, 39, 40, 41, 42,
    53, 54, 55, 56, 57,
    67, 68, 69, 70,
    77,
};

// 每帧算一次的心跳状态，避免每个 LED 重复做除法
static uint16_t heart_phase = 0;  // 0..255
static uint8_t  heart_pulse = 0;  // 0..255，三角波
static bool     heart_ripple_active = false;
static bool     heart_valley_ripple = false;
static uint16_t heart_ripple_phase = 0;  // 纹波独立相位，峰值出现即开始推进

static bool heart_aemeath_led_in_region(uint8_t led) {
    for (uint8_t i = 0; i < HEART_LED_COUNT; i++) {
        if (HEART_LEDS[i] == led) return true;
    }
    return false;
}

static uint8_t heart_aemeath_mix(uint8_t first, uint8_t second, uint8_t amount) {
    return scale8(first, 255 - amount) + scale8(second, amount);
}

static uint8_t heart_aemeath_gradient(uint8_t led) {
    switch (led) {
        case 23: case 24: case 26: case 27: return 0;
        case 37: case 38: case 39: case 40: case 41: case 42: return 40;
        case 53: case 54: case 55: case 56: case 57: return 128;
        case 67: case 68: case 69: case 70: return 216;
        case 77: return 255;
        default: return 0;
    }
}

static void heart_aemeath_calc_center(uint8_t *cx, uint8_t *cy) {
    uint16_t sum_x = 0, sum_y = 0;
    for (uint8_t i = 0; i < HEART_LED_COUNT; i++) {
        sum_x += g_led_config.point[HEART_LEDS[i]].x;
        sum_y += g_led_config.point[HEART_LEDS[i]].y;
    }
    *cx = (uint8_t)(sum_x / HEART_LED_COUNT);
    *cy = (uint8_t)(sum_y / HEART_LED_COUNT);
}

// 根据当前速度更新心跳相位与脉冲（每帧调用一次）。
// 一个周期分为：峰值停顿、下降、谷值停顿、上升；两个极值都停顿一个基础周期。
static void heart_aemeath_update_timing(uint32_t now) {
    uint8_t  speed = rgb_matrix_get_speed();   // 0..255
    // 速度 0 → 慢（周期长），速度 255 → 快（周期短）
    uint16_t period = HEARTBEAT_PERIOD_SLOW_MS -
                      ((uint16_t)speed * (HEARTBEAT_PERIOD_SLOW_MS - HEARTBEAT_PERIOD_FAST_MS) / 255);
    if (period < 32) period = 32;  // 防止除零/周期太小

    const uint16_t half = period / 2;
    const uint16_t pause = period;
    const uint16_t cycle = period * 3;
    const uint16_t elapsed = (uint16_t)(now % cycle);

    if (elapsed < pause) {
        // 峰值停顿：峰值出现时同步触发纹波。
        heart_ripple_active = true;
        heart_valley_ripple = false;
        heart_phase = 0;
        heart_pulse = 255;
        heart_ripple_phase = (uint16_t)((uint32_t)elapsed * 128 / pause);
    } else if (elapsed < pause + half) {
        // 从峰值下降到谷值，纹波随峰值同步向外扩散。
        heart_ripple_active = true;
        heart_valley_ripple = false;
        heart_phase = (uint16_t)((uint32_t)(elapsed - pause) * 128 / half);
        heart_pulse = 255 - heart_phase * 2;
        heart_ripple_phase = (uint16_t)(128 + (uint32_t)(elapsed - pause) * 128 / half);
    } else if (elapsed < pause + half + pause) {
        // 谷值停顿一个周期：谷值出现时同步触发蓝色纹波。
        heart_ripple_active = true;
        heart_valley_ripple = true;
        heart_phase = 128;
        heart_pulse = 0;
        heart_ripple_phase = (uint16_t)((uint32_t)(elapsed - pause - half) * 128 / pause);
    } else {
        // 从谷值回升到下一次峰值。
        heart_ripple_active = true;
        heart_valley_ripple = true;
        heart_phase = (uint16_t)(128 + (uint32_t)(elapsed - pause - half - pause) * 128 / half);
        heart_pulse = (heart_phase - 128) * 2;
        heart_ripple_phase = (uint16_t)(128 + (uint32_t)(elapsed - pause - half - pause) * 128 / half);
    }
}

// --- 星尘 ---
typedef struct {
    uint8_t  led;
    uint32_t birth;
} stardust_t;

static stardust_t stardust[STARDust_COUNT];
static uint32_t  stardust_next_spawn = 0;

static void heart_aemeath_stardust_init(void) {
    for (uint8_t i = 0; i < STARDust_COUNT; i++) {
        stardust[i].led   = 0;
        stardust[i].birth = 0;
    }
    stardust_next_spawn = timer_read32() + 100 + rand() % 500;
}

static uint8_t heart_aemeath_stardust_get(uint8_t led, uint32_t now) {
    for (uint8_t i = 0; i < STARDust_COUNT; i++) {
        if (stardust[i].led == led && stardust[i].birth > 0) {
            uint32_t elapsed = now - stardust[i].birth;
            if (elapsed < STARDust_DECAY_MS) {
                if (elapsed < 300) return 255;
                return 255 - (uint8_t)((uint32_t)255 * (elapsed - 300) / (STARDust_DECAY_MS - 300));
            }
            stardust[i].birth = 0;
        }
    }
    return 0;
}

static void heart_aemeath_stardust_spawn(uint32_t now) {
    if ((int32_t)(now - stardust_next_spawn) < 0) return;

    for (uint8_t i = 0; i < STARDust_COUNT; i++) {
        if (stardust[i].birth == 0) {
            uint8_t attempts  = 0;
            uint8_t candidate;
            do {
                candidate = rand() % RGB_MATRIX_LED_COUNT;
                attempts++;
            } while (heart_aemeath_led_in_region(candidate) && attempts < 50);

            if (!heart_aemeath_led_in_region(candidate)) {
                stardust[i].led   = candidate;
                stardust[i].birth = now;
            }
            break;
        }
    }

    // 生成间隔也跟速度挂钩：速度快时星尘更密
    uint8_t  speed = rgb_matrix_get_speed();
    uint16_t base  = 1200 - (uint16_t)speed * 1000 / 255;  // 1200ms（慢）→ 200ms（快）
    stardust_next_spawn = now + base / 2 + rand() % (base / 2 + 1);
}

// --- 向外扩散的心跳涟漪 ---
static uint8_t heart_aemeath_ripple(uint8_t led_x, uint8_t led_y,
                                     uint8_t heart_cx, uint8_t heart_cy) {
    int16_t dx = (int16_t)led_x - (int16_t)heart_cx;
    int16_t dy = (int16_t)led_y - (int16_t)heart_cy;
    uint16_t dist = (uint16_t)sqrt16((uint32_t)(dx * dx + dy * dy));

    if (dist < 10) return 0;
    if (!heart_ripple_active) return 0;

    uint16_t phase = heart_ripple_phase;

    uint8_t ripple_radius = (uint8_t)((uint16_t)phase * 200 / 256);

    int16_t delta = (int16_t)dist - (int16_t)ripple_radius;
    if (delta < 0) delta = -delta;
    if (delta >= RIPPLE_WIDTH) return 0;

    uint8_t brightness = (uint8_t)((uint16_t)(RIPPLE_WIDTH - delta) * 255 / RIPPLE_WIDTH);

    if (dist > 60) {
        uint16_t fade = (uint16_t)(dist - 60) * 255 / 140;
        if (fade > 255) fade = 255;
        brightness = scale8(brightness, 255 - (uint8_t)fade);
    }

    if (phase > 200) brightness = 0;

    return brightness;
}

// --- 心形 LED 颜色：基础渐变 + 向纯红呼吸 ---
static void heart_aemeath_set_heart_color(uint8_t led, uint8_t max_intensity) {
    uint8_t gradient = heart_aemeath_gradient(led);

    uint8_t base_r = heart_aemeath_mix(HEART_PINK_R, HEART_BLUE_R, gradient);
    uint8_t base_g = heart_aemeath_mix(HEART_PINK_G, HEART_BLUE_G, gradient);
    uint8_t base_b = heart_aemeath_mix(HEART_PINK_B, HEART_BLUE_B, gradient);

    uint8_t pulse = heart_pulse;

    uint8_t r = heart_aemeath_mix(base_r, HEART_RED_R, pulse);
    uint8_t g = heart_aemeath_mix(base_g, HEART_RED_G, pulse);
    uint8_t b = heart_aemeath_mix(base_b, HEART_RED_B, pulse);

    rgb_matrix_set_color(led, scale8(r, max_intensity), scale8(g, max_intensity), scale8(b, max_intensity));
}

// --- 心外 LED 颜色 ---
static void heart_aemeath_set_bg_color(uint8_t led, uint8_t heart_cx, uint8_t heart_cy,
                                        uint8_t max_intensity) {
    uint8_t bg_r = scale8(HEART_BG_R, HEART_BG_BRIGHTNESS);
    uint8_t bg_g = scale8(HEART_BG_G, HEART_BG_BRIGHTNESS);
    uint8_t bg_b = scale8(HEART_BG_B, HEART_BG_BRIGHTNESS);

    uint8_t ripple = heart_aemeath_ripple(
        g_led_config.point[led].x, g_led_config.point[led].y,
        heart_cx, heart_cy);

    const uint8_t ripple_r = heart_valley_ripple ? HEART_VALLEY_RIPPLE_R : HEART_RIPPLE_R;
    const uint8_t ripple_g = heart_valley_ripple ? HEART_VALLEY_RIPPLE_G : HEART_RIPPLE_G;
    const uint8_t ripple_b = heart_valley_ripple ? HEART_VALLEY_RIPPLE_B : HEART_RIPPLE_B;
    uint8_t ripple_color_r = scale8(ripple, ripple_r);
    uint8_t ripple_color_g = scale8(ripple, ripple_g);
    uint8_t ripple_color_b = scale8(ripple, ripple_b);

    uint8_t dust = heart_aemeath_stardust_get(led, timer_read32());
    dust = scale8(dust, STARDust_MAX_BRIGHT);
    uint8_t dust_r = scale8(dust, HEART_STARDUST_R);
    uint8_t dust_g = scale8(dust, HEART_STARDUST_G);
    uint8_t dust_b = scale8(dust, HEART_STARDUST_B);

    uint8_t r = qadd8(bg_r, ripple_color_r);
    uint8_t g = qadd8(bg_g, ripple_color_g);
    uint8_t b = qadd8(bg_b, ripple_color_b);

    r = qadd8(r, scale8(dust_r, 255 - r));
    g = qadd8(g, scale8(dust_g, 255 - g));
    b = qadd8(b, scale8(dust_b, 255 - b));

    rgb_matrix_set_color(led, scale8(r, max_intensity), scale8(g, max_intensity), scale8(b, max_intensity));
}

static bool HEART_AEMEATH(effect_params_t *params) {
    const uint8_t max_intensity = rgb_matrix_config.hsv.v;
    static uint8_t heart_cx = 0, heart_cy = 0;

    if (params->init) {
        heart_aemeath_calc_center(&heart_cx, &heart_cy);
        heart_aemeath_stardust_init();
    }

    if (max_intensity == 0) return false;

    uint32_t now = timer_read32();
    heart_aemeath_update_timing(now);
    heart_aemeath_stardust_spawn(now);

    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        if (heart_aemeath_led_in_region(i)) {
            heart_aemeath_set_heart_color(i, max_intensity);
        } else {
            heart_aemeath_set_bg_color(i, heart_cx, heart_cy, max_intensity);
        }
    }

    return false;
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
