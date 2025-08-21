#ifndef __BUTTONS_H
#define __BUTTONS_H

#include <stdint.h>
#include "driver/gpio.h"

struct ec11_t {
    gpio_num_t pinA;
    gpio_num_t pinB;
    uint8_t last_state;
    uint8_t stable_count;
    uint8_t stable_state;
    int value;
};

struct buttons_t {

    struct ec11_t ec11[5];
    int click[5];
};

struct buttons_t *buttons_init();
int button_rot_get(struct buttons_t *buttons, int n);
void button_read_all(struct buttons_t *buttons);
void gpio_conf_input(int gpio);

#endif