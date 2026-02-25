/* Copyright 2021 Glorious, LLC <salman@pcgamingrace.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "rgb_utils.h"
#include <math.h>


// --- SETTINGS OG DEFINITIONER ---
#define CAPS_LOCK_LED_INDEX 3

// Arrays til side-LEDs
static const uint8_t side_leds[] = {
   LED_L1, LED_L2, LED_L3, LED_L4, LED_L5, LED_L6, LED_L7, LED_L8, // Venstre side
   LED_R1, LED_R2, LED_R3, LED_R4, LED_R5, LED_R6, LED_R7, LED_R8  // Højre side
};
static const uint8_t side_led_count = sizeof(side_leds) / sizeof(side_leds[0]);


// Indstillinger for Bølgen (Standard float værdier)
#define WAVE_SPEED 0.00075f   // Hastighed
#define WAVE_SCALE 0.0150f    // Bredde (Horisontal)

#define WAVE_SCALE_V 0.0600f  // Bredde (Vertikal) - Ca. 4x højere end den horisontale (højere tal = tættere bølge)


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//      ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12	     Prt           Rotary(Mute)
//      ~        1        2        3        4        5        6        7        8        9        0         -       (=)	     BackSpc           Del
//      Tab      Q        W        E        R        T        Y        U        I        O        P        [        ]                          PgUp
//      Caps     A        S        D        F        G        H        J        K        L        ;        "        #        Enter             PgDn
//      Sh_L     /        Z        X        C        V        B        N        M        ,        .        ?                 Sh_R     Up       End
//      Ct_L     Win_L    Alt_L                               SPACE                               Alt_R    FN       Ct_R     Left     Down     Right


    // The FN key by default maps to a momentary toggle to layer 1 to provide access to the QK_BOOT key (to put the board into bootloader mode). Without
    // this mapping, you have to open the case to hit the button on the bottom of the PCB (near the USB cable attachment) while plugging in the USB
    // cable to get the board into bootloader mode - definitely not fun when you're working on your QMK builds. Remove this and put it back to KC_RGUI
    // if that's your preference.
    //
    // To put the keyboard in bootloader mode, use FN+backspace. If you accidentally put it into bootloader, you can just unplug the USB cable and
    // it'll be back to normal when you plug it back in.
    //
    // This keyboard defaults to 6KRO instead of NKRO for compatibility reasons (some KVMs and BIOSes are incompatible with NKRO).
    // Since this is, among other things, a "gaming" keyboard, a key combination to enable NKRO on the fly is provided for convenience.
    // Press Fn+N to toggle between 6KRO and NKRO. This setting is persisted to the EEPROM and thus persists between restarts.
    // [0] DEFAULT LAYER
        [0] = LAYOUT(
            KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,           KC_F15,
            KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,          KC_MPLY,
            KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC,                   KC_MNXT,
            KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,           KC_MPRV,
            KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_RSFT, KC_UP,   KC_F16,
            KC_LCTL, KC_LGUI, KC_LALT,                            KC_SPC,                             KC_RALT, MO(1),   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
        ),

        // [1] FUNCTION LAYER
        [1] = LAYOUT(
            _______, _______, _______, KC_CALC, RM_TOGG, RM_VALD, RM_VALU, _______, _______, _______, KC_MUTE, KC_VOLD, KC_VOLU, _______,          _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT,          _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, RM_NEXT, _______,
            _______, _______, _______,                            _______,                            _______, _______, _______, RM_SPDD, RM_PREV, RM_SPDU
        ),

        // [2] LAYER 2 (Empty/White)
        [2] = LAYOUT(
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______,
            _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______, _______
        ),

        // [3] LAYER 3 (Empty/Side LEDs)
        [3] = LAYOUT(
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,                   _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,
            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______,
            _______, _______, _______,                            _______,                            _______, _______, _______, _______, _______, _______
        )
};
// clang-format on


// --- RGB LOGIK ---
bool rgb_matrix_indicators_user(void) {
    uint8_t total = RGB_MATRIX_LED_COUNT;
    bool caps_on = host_keyboard_led_state().caps_lock;
    uint8_t layer = get_highest_layer(layer_state);

    // Hent den globale lysstyrke
    uint8_t val = rgb_matrix_get_val();

    // Caps Lock Indikator
    if (caps_on) {
        rgb_matrix_set_color(CAPS_LOCK_LED_INDEX, val, 0, 0);
    }

    // ========== LAYER 2 – HELT HVID ==========
    if (layer == 2) {
        for (uint8_t i = 0; i < total; i++) {
            rgb_matrix_set_color(i, val, val, val);
        }
        return false;
    }

    // ========== LAYER 3 – KUN SIDE-LED (Vertikal Bølge) ==========
    if (layer == 3) {
        uint32_t time = timer_read32();

        for (uint8_t i = 0; i < total; i++) {
            if (caps_on && i == CAPS_LOCK_LED_INDEX) {
                continue;
            }

            bool is_side_led = false;
            for (uint8_t j = 0; j < side_led_count; j++) {
                if (i == side_leds[j]){
                    is_side_led = true;
                    break;
                }
            }

            if (is_side_led) {
                led_point_t led = g_led_config.point[i];

                // --- HER ER ÆNDRINGEN FOR LODRET BØLGE ---
                // Vi bruger led.y i stedet for led.x
                // Vi bruger WAVE_SCALE_V i stedet for WAVE_SCALE
                float wave = sinf((led.y * WAVE_SCALE_V) - (time * WAVE_SPEED)); // <-- ÆNDRET HER (led.y og SCALE_V)

                float t = (wave + 1.0f) * 0.5f;

                uint8_t hue = 140 + (uint8_t)(t * 120.0f);
                uint8_t sat = rgb_matrix_get_sat();

                HSV hsv = { hue, sat, val };
                RGB rgb = hsv_to_rgb(hsv);
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);

            } else {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
        }
        return false;
    }

    // ========== LAYER 0 – SEAMLESS WAVE (Horisontal Standard) ==========
    // Denne rører vi ikke, så den forbliver vandret som før.
    if (layer == 0) {
        uint32_t time = timer_read32();

        for (uint8_t i = 0; i < total; i++) {
            if (caps_on && i == CAPS_LOCK_LED_INDEX) {
                continue;
            }
            led_point_t led = g_led_config.point[i];

            // Denne bruger stadig led.x og den gamle WAVE_SCALE
            float wave = sinf((led.x * WAVE_SCALE) - (time * WAVE_SPEED));
            float t = (wave + 1.0f) * 0.5f;

            uint8_t hue = 140 + (uint8_t)(t * 120.0f);
            uint8_t sat = rgb_matrix_get_sat();

            HSV hsv = { hue, sat, val };
            RGB rgb = hsv_to_rgb(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        }
        return false;
    }

    return true;
}



// --- ENCODER LOGIK (Simpel F13/F14) ---
bool encoder_update_user(uint8_t index, bool clockwise) {

    // --- Layer Rotation når FN holdes nede ---
    if (layer_state_is(1)) {
        const uint8_t rot_layers[] = {0, 2, 3};
        const uint8_t count = 3;

        uint8_t current = get_highest_layer(layer_state);
        uint8_t pos = 0;

            for (uint8_t i = 0; i < count; i++) {
                if (rot_layers[i] == current) {
                pos = i;
                break;
           }
       }

        pos = clockwise ? (pos + 1) % count : (pos + count - 1) % count;

        layer_off(0);
        layer_off(2);
        layer_off(3);
        layer_on(rot_layers[pos]);

        return false;
    }

   // --- Normal kontrol (Layer 0) ---
   // Dette virker nu perfekt sammen med pynput!
    if (clockwise) {
        tap_code(KC_F14);
    }
    else {
        tap_code(KC_F13);
    }
    return false;
}
