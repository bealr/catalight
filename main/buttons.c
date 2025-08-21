#include "buttons.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


struct buttons_t *buttons_init() {

    int i;

    struct buttons_t *buttons = (struct buttons_t *) malloc(sizeof(struct buttons_t));

    gpio_reset_pin(1);
    gpio_set_direction(1, GPIO_MODE_INPUT);
    gpio_reset_pin(2);
    gpio_set_direction(2, GPIO_MODE_INPUT);
    gpio_reset_pin(3);
    gpio_set_direction(3, GPIO_MODE_INPUT);
    gpio_reset_pin(4);
    gpio_set_direction(4, GPIO_MODE_INPUT);
    gpio_reset_pin(5);
    gpio_set_direction(5, GPIO_MODE_INPUT);

    gpio_reset_pin(39);
    gpio_set_direction(39, GPIO_MODE_INPUT);
    gpio_reset_pin(40);
    gpio_set_direction(40, GPIO_MODE_INPUT);

    gpio_reset_pin(37);
    gpio_set_direction(37, GPIO_MODE_INPUT);
    gpio_reset_pin(38);
    gpio_set_direction(38, GPIO_MODE_INPUT);

    gpio_reset_pin(34);
    gpio_set_direction(34, GPIO_MODE_INPUT);
    gpio_reset_pin(36);
    gpio_set_direction(36, GPIO_MODE_INPUT);

    gpio_reset_pin(18);
    gpio_set_direction(18, GPIO_MODE_INPUT);
    gpio_reset_pin(21);
    gpio_set_direction(21, GPIO_MODE_INPUT);

    gpio_reset_pin(16);
    gpio_set_direction(16, GPIO_MODE_INPUT);
    gpio_reset_pin(17);
    gpio_set_direction(17, GPIO_MODE_INPUT);

    for (i=0;i<5;i++) {
        buttons->ec11[i].value = 0;
        buttons->click[i] = 0;
    }

    buttons->ec11[0].pinA = 39;
    buttons->ec11[0].pinB = 40;

    buttons->ec11[1].pinA = 37;
    buttons->ec11[1].pinB = 38;

    buttons->ec11[2].pinA = 36;
    buttons->ec11[2].pinB = 34;

    buttons->ec11[3].pinA = 18;
    buttons->ec11[3].pinB = 21;

    buttons->ec11[4].pinA = 16;
    buttons->ec11[4].pinB = 17;

    return buttons;
}

int button_rot_get(struct buttons_t *buttons, int n) {

    uint8_t state_a = gpio_get_level(buttons->ec11[n].pinA);
    uint8_t state_b = gpio_get_level(buttons->ec11[n].pinB);

    int result = 0;

    if (buttons->ec11[n].last_state != state_a) {

        if (state_a == state_b)
            result = -1;
        else
            result = 1;
    }

    buttons->ec11[n].last_state = state_a;

    if (result != 0) {

        vTaskDelay(pdMS_TO_TICKS(30));
        buttons->ec11[n].last_state = gpio_get_level(buttons->ec11[n].pinA);

    }
    return result;
}


void button_read_all(struct buttons_t *buttons) {

    int i;
    int val;

    for (i=0;i<5;i++) {
        buttons->ec11[i].value += button_rot_get(buttons, i);
    }

    for (i=0;i<5;i++) {
        val = gpio_get_level(i+1);

        if (val == 0) {

            printf("button %d DOWN\n", i);
            while(gpio_get_level(i+1) == 0) vTaskDelay(pdMS_TO_TICKS(30));
            buttons->click[i] = 1;
            printf("button %d UP\n", i);
        }
    }
}
