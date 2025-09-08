#include "buttons.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void gpio_conf_input(int gpio) {

    gpio_reset_pin(gpio);
    gpio_set_direction(gpio, GPIO_MODE_INPUT);
    gpio_set_pull_mode(gpio, GPIO_PULLUP_ONLY);
}

struct buttons_t *buttons_init() {

    int i;
    struct buttons_t *buttons = (struct buttons_t *) malloc(sizeof(struct buttons_t));

    // buttons
    gpio_conf_input(19);
    gpio_conf_input(18);
    gpio_conf_input(5);
    gpio_conf_input(17);
    gpio_conf_input(16);
    buttons->button_io[2] = 19;
    buttons->button_io[1] = 18;
    buttons->button_io[0] = 5;
    buttons->button_io[4] = 17;
    buttons->button_io[3] = 16;

    // coders
    gpio_conf_input(34);
    gpio_conf_input(35);
    gpio_conf_input(32);
    gpio_conf_input(33);
    gpio_conf_input(25);
    gpio_conf_input(26);
    gpio_conf_input(27);
    gpio_conf_input(14);
    gpio_conf_input(12);
    gpio_conf_input(13);

    buttons->ec11[0].pinA = 34;
    buttons->ec11[0].pinB = 35;

    buttons->ec11[1].pinA = 32;
    buttons->ec11[1].pinB = 33;

    buttons->ec11[2].pinA = 25;
    buttons->ec11[2].pinB = 26;

    buttons->ec11[3].pinA = 27;
    buttons->ec11[3].pinB = 14;

    buttons->ec11[4].pinA = 12;
    buttons->ec11[4].pinB = 13;

    // Reset
    for (i=0;i<5;i++) {
        buttons->ec11[i].value = 0;
        buttons->click[i] = 0;
    }

    return buttons;
}

int button_rot_get(struct buttons_t *buttons, int n) {

struct ec11_t *enc = &buttons->ec11[n];

    uint8_t state_a = gpio_get_level(enc->pinA);
    uint8_t state_b = gpio_get_level(enc->pinB);
    uint8_t curr_state = (state_a << 1) | state_b;

    int result = 0;

    // Détection uniquement au retour sur "00"
    if (curr_state == 0b00 && enc->last_state != 0b00) {
        printf("move on pot %d\n", n);
        // Si la transition précédente était 10 → cran dans un sens
        if (enc->last_state == 0b10) {
            result = -1;   // horaire
        }
        // Si c'était 01 → cran dans l'autre sens
        else if (enc->last_state == 0b01) {
            result = +1;   // antihoraire
        }
    }

    enc->last_state = curr_state;
    return result;
}


void button_read_all(struct buttons_t *buttons) {

    int i;
    int val;

    for (i=0;i<5;i++) {
        buttons->ec11[i].value += button_rot_get(buttons, i);

        if (buttons->ec11[i].value < 0) buttons->ec11[i].value = 0;
        if (buttons->ec11[i].value > 93) buttons->ec11[i].value = 93;

        val = gpio_get_level(buttons->button_io[i]);
        if (val == 0) {

            printf("button %d DOWN\n", i);
            while(gpio_get_level(buttons->button_io[i]) == 0) vTaskDelay(pdMS_TO_TICKS(30));
            buttons->click[i] = 1;
            printf("button %d UP\n", i);
        }
    }
}
