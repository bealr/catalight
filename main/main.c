#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

#include "oled.h"
#include "buttons.h"
#include "display.h"
#include "defines.h"

static void IRAM_ATTR encoder_timer_callback(void *arg)
{
    struct buttons_t *buttons = (struct buttons_t *) arg;
    button_read_all(buttons);
}

void encoder_timer_start(struct buttons_t *buttons)
{
    const esp_timer_create_args_t timer_args = {
        .callback = &encoder_timer_callback,
        .arg = buttons,
        .dispatch_method = ESP_TIMER_TASK, // ou ESP_TIMER_ISR
        .name = "encoder_poll"
    };

    esp_timer_handle_t timer;
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer, 50));
}

int process_rgb(int color, struct buttons_t *buttons, int light_id, struct lights_t *lights) {

    static int save[5];

    if (buttons->ec11[color].value < 0) buttons->ec11[color].value = 0;
    if (buttons->ec11[color].value > 254) buttons->ec11[color].value = 254;


    switch (color) {
        case 2:
            lights->light[light_id].r = buttons->ec11[color].value;
            break;
        case 1:
            lights->light[light_id].g = buttons->ec11[color].value;
            break;
        case 0:
            lights->light[light_id].b = buttons->ec11[color].value;
            break;
        case 4:
            lights->light[light_id].w = buttons->ec11[color].value;
            break;
        case 3:
            lights->light[light_id].y = buttons->ec11[color].value;
            break;

        default:
            (void) lights;
    }

    if (save[color] != buttons->ec11[color].value) {
        save[color] = buttons->ec11[color].value;

        printf("hoho : %d -- %d\n", lights->light[light_id].r, light_id);
        return 1;
    }

    return 0;
}

void app_main(void)
{
    ssd1306_handle_t dev;
    struct buttons_t *buttons;
    struct lights_t *lights;

    dev = oled_init();
    buttons = buttons_init();
    lights = (struct lights_t *) malloc(sizeof(struct lights_t));

    int force_refresh = 0;;
    int light_selected = 0;
    int current_page = 0;
    // 0: welcome
    // 1: select light
    // 2: rgb

    int i;
    vTaskDelay(pdMS_TO_TICKS(1000));
    for (i=0;i<5;i++) {
        buttons->ec11[i].value = 0;
        buttons->click[i] = 0;
    }

    encoder_timer_start(buttons);

    while (1) {

        if (current_page == 0) {
            dislay_welcome(dev);
            if (buttons->click[0]) {
                buttons->click[0] = 0; // ACK
                current_page = 1;
                light_selected = 0;
                buttons->ec11[0].value = 0;
                force_refresh = 1;
            }
        }

        if (current_page == 1) {

            if (buttons->ec11[0].value < 0) buttons->ec11[0].value = 0;
            if (buttons->ec11[0].value > 20) buttons->ec11[0].value = 20;

            light_selected = buttons->ec11[2].value;

            dislay_light_select(dev, &light_selected, force_refresh);
            force_refresh = 0;
            if (buttons->click[0]) {
                buttons->click[0] = 0; // ACK
                current_page = 2;
                force_refresh = 1;
            }
        }

        if (current_page == 2) {

            if (process_rgb(0, buttons, light_selected, lights) +
                process_rgb(1, buttons, light_selected, lights) +
                process_rgb(2, buttons, light_selected, lights) +
                process_rgb(3, buttons, light_selected, lights) +
                process_rgb(4, buttons, light_selected, lights) + force_refresh) {

                    force_refresh = 0;
                    printf("do refresh!! \n");
                    // do refresh !
                    dislay_rgb_ctrl(dev, light_selected, lights);
            }

            if (buttons->click[0]) {
                buttons->click[0] = 0; // ACK
                current_page = 1;
                force_refresh = 1;

                buttons->ec11[2].value = light_selected;
            }
        }
            
    }
}


