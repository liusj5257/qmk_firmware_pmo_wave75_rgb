/* Copyright 2023 Finalkey
 * Copyright 2023 LiWenLiu <https://github.com/Linger7857>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../../lib/rdr_lib/rdr_common.h"
#include "usb_device_state.h"

void matrix_io_delay(void) {
}

void matrix_output_select_delay(void) {
}

void matrix_output_unselect_delay(uint8_t line, bool key_pressed) {
}

led_config_t g_led_config = { {
	{ 0        , 1        , 2        , 3        , 4        , 5        , 6        , 7        , 8        , 9        , 10       , 11       , 12       , NO_LED   , NO_LED   , NO_LED    },
	{ 15       , 16       , 17       , 18       , 19       , 20       , 21       , 22       , 23       , 24       , 25       , 26       , 27       , 28       , NO_LED   , 14        },
	{ 30       , 31       , 32       , 33       , 34       , 35       , 36       , 37       , 38       , 39       , 40       , 41       , 42       , 43       , 13       , 29        },
	{ 45       , 47       , 48       , 49       , 50       , 51       , 52       , 53       , 54       , 55       , 56       , 57       , NO_LED   , 58       , 44       , 59        },
	{ 60       , NO_LED   , 61       , 62       , 63       , 64       , 65       , 66       , 67       , 68       , 69       , 70       , NO_LED   , 71       , 72       , NO_LED    },
	{ 73       , 74       , 75       , NO_LED   , NO_LED   , 76       , NO_LED   , NO_LED   , NO_LED   , 77       , 78       , NO_LED   , NO_LED   , 79       , 80       , 81        },
    { NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED   , NO_LED    }
},{
    // "Fine-tuned" complex configuration
                { 0,   10},   { 15,  10},  { 30,  10},  { 45,  10}, { 60,  10}, { 75,  10}, { 90, 10}, { 105, 10}, { 120, 10}, { 135, 10}, { 150, 10}, { 165, 10}, { 180, 10}, { 195, 10}, { 208, 10},
                { 0,   20},   { 15,  20},  { 30,  20},  { 45,  20}, { 60,  20}, { 75,  20}, { 90, 20}, { 105, 20}, { 120, 20}, { 135, 20}, { 150, 20}, { 165, 20}, { 180, 20}, { 195, 20}, { 208, 20},
                { 0,   30},   { 15,  30},  { 30,  30},  { 45,  30}, { 60,  30}, { 75,  30}, { 90, 30}, { 105, 30}, { 120, 30}, { 135, 30}, { 150, 30}, { 165, 30}, { 180, 30}, { 195, 30}, { 208, 30},
    { 0,   40}, { 0,   40},   { 15,  40},  { 30,  40},  { 45,  40}, { 60,  40}, { 75,  40}, { 90, 40}, { 105, 40}, { 120, 40}, { 135, 40}, { 150, 40}, { 165, 40},             { 195, 40}, { 208, 40},
                { 0,   50},                { 15,  50},  { 30,  50}, { 45,  50}, { 60,  50}, { 75, 50}, { 90, 50},  { 105, 50}, { 120, 50}, { 135, 50}, { 150, 50}, { 165, 50}, { 180, 50},
                { 0,   60},   { 15,  60},  { 30,  60},                          { 75,  60},                                    { 135, 60}, { 150, 60},             { 180, 60}, { 195, 60}, { 208, 60},

                { 225, 65},   { 225,  65}, { 225,  65}, { 225,  65}
}, {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,    1,
    1, 1, 1,       1,          1, 1,    1, 1, 1,

    0, 0, 0, 0
} };

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    User_Led_Show();
    return false;
}

void notify_usb_device_state_change_user(struct usb_device_state usb_state)  {
    if (Keyboard_Info.Key_Mode == QMK_USB_MODE) {
        if (usb_state.configure_state == USB_DEVICE_STATE_CONFIGURED) {
            Usb_If_Ok_Led = true;
        } else {
            Usb_If_Ok_Led = false;
        }
    } else {
        Usb_If_Ok_Led = false;
    }
}

void housekeeping_task_user(void) {
    User_Keyboard_Reset();
    es_chibios_user_idle_loop_hook();
}

void board_init(void) {
    User_Keyboard_Init();
}

void keyboard_post_init_user(void) {
    User_Keyboard_Post_Init();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {   /*键盘只要有按键按下就会调用此函数*/
    Usb_Change_Mode_Delay = 0;                                      /*只要有按键就不会进入休眠*/
    Usb_Change_Mode_Wakeup = false;

    return Key_Value_Dispose(keycode, record);
}

