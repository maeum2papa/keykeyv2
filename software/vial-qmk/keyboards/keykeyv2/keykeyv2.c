#include "keykeyv2.h"

void keyboard_pre_init_kb(void) {
    gpio_set_pin_input_high(GP12);
    gpio_set_pin_input_high(GP13);
    gpio_set_pin_input_high(GP14);
    gpio_set_pin_input_high(GP15);
    gpio_set_pin_output(GP28);
    gpio_write_pin_low(GP28);
    keyboard_pre_init_user();
}

bool led_update_kb(led_t led_state) {
    if (led_update_user(led_state)) {
        gpio_write_pin(GP28, led_state.caps_lock);
    }
    return true;
}
