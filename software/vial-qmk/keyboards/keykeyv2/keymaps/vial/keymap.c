#include QMK_KEYBOARD_H


void matrix_scan_user(void) {
    static bool key_pressed[4] = {false, false, false, false};

    bool p12 = !gpio_read_pin(GP12);
    bool p13 = !gpio_read_pin(GP13);
    bool p14 = !gpio_read_pin(GP14);
    bool p15 = !gpio_read_pin(GP15);

    // UP    = GP12 + GP14
    if (p12 && p14) {
        if (!key_pressed[0]) { register_code(KC_UP);      key_pressed[0] = true; }
    } else {
        if (key_pressed[0])  { unregister_code(KC_UP);    key_pressed[0] = false; }
    }

    // DOWN  = GP13 + GP15
    if (p13 && p15) {
        if (!key_pressed[1]) { register_code(KC_DOWN);    key_pressed[1] = true; }
    } else {
        if (key_pressed[1])  { unregister_code(KC_DOWN);  key_pressed[1] = false; }
    }

    // LEFT  = GP12 + GP13
    if (p12 && p13) {
        if (!key_pressed[2]) { register_code(KC_LEFT);    key_pressed[2] = true; }
    } else {
        if (key_pressed[2])  { unregister_code(KC_LEFT);  key_pressed[2] = false; }
    }

    // RIGHT = GP14 + GP15
    if (p14 && p15) {
        if (!key_pressed[3]) { register_code(KC_RIGHT);   key_pressed[3] = true; }
    } else {
        if (key_pressed[3])  { unregister_code(KC_RIGHT); key_pressed[3] = false; }
    }
}


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
        KC_LALT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,
        KC_LCTL, KC_CAPS, KC_SPC,  KC_LBRC, KC_BSLS, KC_TRNS,

        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_LABK,
        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_COMM,
        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_TRNS, KC_EQL,  KC_RBRC, KC_ENT,  KC_BSPC, KC_RALT
    ),
    [1] = LAYOUT(
        KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,

        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
        _______, _______, _______, KC_MINS, KC_EQL,  _______,
        _______, _______, _______, KC_LBRC, KC_RBRC, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END
    ),
    [2] = LAYOUT(
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        QK_BOOT, _______, _______, _______, _______, _______,

        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______
    ),
};
