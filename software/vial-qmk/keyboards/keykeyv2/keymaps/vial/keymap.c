#include QMK_KEYBOARD_H

#ifdef SPLIT_KEYBOARD
#    include "transactions.h"
#    include "split_common/split_util.h"
#endif

#ifdef CONSOLE_ENABLE
#    include "print.h"
#endif

typedef struct {
    uint8_t pressed_bits; // bit0..3 => GP12..GP15 (1 = pressed)
} nav5_gpio_state_t;

static nav5_gpio_state_t g_nav5_local  = {0};
static nav5_gpio_state_t g_nav5_remote = {0};

static nav5_gpio_state_t nav5_read_gpio_state(void) {
    nav5_gpio_state_t s = {0};
    if (!gpio_read_pin(GP12)) s.pressed_bits |= (1u << 0);
    if (!gpio_read_pin(GP13)) s.pressed_bits |= (1u << 1);
    if (!gpio_read_pin(GP14)) s.pressed_bits |= (1u << 2);
    if (!gpio_read_pin(GP15)) s.pressed_bits |= (1u << 3);
    return s;
}

#ifdef SPLIT_KEYBOARD
static void nav5_gpio_sync_slave(uint8_t initiator2target_buffer_size, const void *initiator2target_buffer, uint8_t target2initiator_buffer_size, void *target2initiator_buffer) {
    (void)initiator2target_buffer_size;
    (void)initiator2target_buffer;

    if (target2initiator_buffer_size >= sizeof(nav5_gpio_state_t) && target2initiator_buffer) {
        *(nav5_gpio_state_t *)target2initiator_buffer = g_nav5_local;
    }
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(NAV5_GPIO_SYNC, nav5_gpio_sync_slave);

#    if defined(CONSOLE_ENABLE)
    debug_enable = true;
    uprintf("[init] left=%u master=%u\n", is_keyboard_left(), is_keyboard_master());
#    endif
}

void housekeeping_task_user(void) {
    // Always keep local state fresh so the slave handler can respond immediately.
    g_nav5_local = nav5_read_gpio_state();

#    if defined(CONSOLE_ENABLE)
    // 슬레이브에서도 nav 핀 상태 로깅 (눌렸을 때만)
    if (!is_keyboard_master() && g_nav5_local.pressed_bits != 0) {
        static uint8_t last_local = 0;
        if (g_nav5_local.pressed_bits != last_local) {
            last_local = g_nav5_local.pressed_bits;
            uprintf("[slave] local=0x%02X\n", g_nav5_local.pressed_bits);
        }
    }
#    endif

    if (!is_keyboard_master()) {
        return;
    }

    // Debug: print nav remote bits only when non-zero.
#    if defined(CONSOLE_ENABLE)
    static uint8_t last_remote_bits = 0;
    if (g_nav5_remote.pressed_bits != last_remote_bits) {
        last_remote_bits = g_nav5_remote.pressed_bits;
        uprintf("[nav] remote=0x%02X\n", g_nav5_remote.pressed_bits);
    }
#    endif

    // Poll the other half at a modest rate to avoid impacting key scanning.
    static uint32_t last_sync = 0;
    if (timer_elapsed32(last_sync) < 10) {
        return;
    }
    last_sync = timer_read32();

    nav5_gpio_state_t remote = {0};
    if (transaction_rpc_recv(NAV5_GPIO_SYNC, sizeof(remote), &remote)) {
        g_nav5_remote = remote;
#    if defined(CONSOLE_ENABLE)
        uprintf("[split] rpc ok remote=0x%02X\n", remote.pressed_bits);
#    endif
    } else {
        // Fail-safe: release any remote-generated presses if comms drop.
        g_nav5_remote.pressed_bits = 0;
#    if defined(CONSOLE_ENABLE)
        uprintf("[split] rpc FAIL\n");
#    endif
    }
}
#endif


void matrix_scan_user(void) {
#ifdef SPLIT_KEYBOARD
    if (!is_keyboard_master()) {
        return;
    }
#endif
    static bool key_pressed[4] = {false, false, false, false};

    // Combine nav input from both halves (master-local + remote).
    nav5_gpio_state_t local = nav5_read_gpio_state();
#ifdef SPLIT_KEYBOARD
    uint8_t bits = local.pressed_bits | g_nav5_remote.pressed_bits;
#else
    uint8_t bits = local.pressed_bits;
#endif

    // PCB 설계상 대각 스위치 2개가 동시에 눌려야 각 방향이 활성화됨
    // bit0=GP12, bit1=GP13, bit2=GP14, bit3=GP15
    bool p0 = bits & (1u << 0);
    bool p1 = bits & (1u << 1);
    bool p2 = bits & (1u << 2);
    bool p3 = bits & (1u << 3);

    // UP    = GP12 + GP14
    if (p0 && p2) {
        if (!key_pressed[0]) { register_code(KC_UP);      key_pressed[0] = true; }
    } else {
        if (key_pressed[0])  { unregister_code(KC_UP);    key_pressed[0] = false; }
    }

    // DOWN  = GP13 + GP15
    if (p1 && p3) {
        if (!key_pressed[1]) { register_code(KC_DOWN);    key_pressed[1] = true; }
    } else {
        if (key_pressed[1])  { unregister_code(KC_DOWN);  key_pressed[1] = false; }
    }

    // LEFT  = GP12 + GP13
    if (p0 && p1) {
        if (!key_pressed[2]) { register_code(KC_LEFT);    key_pressed[2] = true; }
    } else {
        if (key_pressed[2])  { unregister_code(KC_LEFT);  key_pressed[2] = false; }
    }

    // RIGHT = GP14 + GP15
    if (p2 && p3) {
        if (!key_pressed[3]) { register_code(KC_RIGHT);   key_pressed[3] = true; }
    } else {
        if (key_pressed[3])  { unregister_code(KC_RIGHT); key_pressed[3] = false; }
    }
}

#ifdef CONSOLE_ENABLE
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        uprintf("[key] kc=0x%04X row=%u col=%u\n", keycode, record->event.key.row, record->event.key.col);
    }
    return true;
}
#endif


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
        KC_LALT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,
        KC_LCTL, KC_CAPS, KC_SPC,  KC_LBRC, KC_BSLS, KC_TRNS,

        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV,
        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS,
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
