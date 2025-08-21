#include "display.h"
#include <string.h>
#include "defines.h"

void dislay_welcome(ssd1306_handle_t dev) {

    char str[50];

    sprintf(str, "Fuck !!");

    ssd1306_clear_screen(dev, false);
    ssd1306_draw_string(dev, 0, 0, (unsigned char *)str, 12, true);
    ssd1306_refresh_gram(dev);
}

void dislay_light_select(ssd1306_handle_t dev, int *light_selected, int force_refresh) {

    char str[50];

    static int save_light_selected;

    if (save_light_selected == *light_selected && force_refresh == 0)
        return ;

    save_light_selected = *light_selected;


    ssd1306_clear_screen(dev, false);

    sprintf(str, "Light Selection");
    ssd1306_draw_string(dev, 17, 0, (unsigned char *)str, 12, true);

    ssd1306_draw_line(dev, 0, 13, 128, 12);

    sprintf(str, "Current selected : %d", *light_selected);
    ssd1306_draw_string(dev, 0, 20, (unsigned char *)str, 12, true);

    sprintf(str, "RSSI : %d", -70);
    ssd1306_draw_string(dev, 0, 32, (unsigned char *)str, 12, true);

    ssd1306_refresh_gram(dev);
}


void dislay_rgb_ctrl(ssd1306_handle_t dev, int light_selected, struct lights_t *lights) {
    
    char str[50];

    int padding = 0;
    int bar_size;

    ssd1306_clear_screen(dev, false);

    sprintf(str, "Light : %d", light_selected);
    ssd1306_draw_string(dev, 0, 0, (unsigned char *)str, 12, true);

    ssd1306_draw_line(dev, 0, 13, 128, 12);

    padding = 10;
    bar_size = lights->light[light_selected].r / 1;

    printf("bar size r -- %d: %d\n", light_selected, lights->light[light_selected].r);

    ssd1306_fill_rectangle(dev, padding, 20, padding+4, bar_size +20, 1);
    sprintf(str, "r");
    ssd1306_draw_string(dev, padding, 52, (unsigned char *)str, 12, true);

    padding += 24;
    bar_size = lights->light[light_selected].g / 1;
    ssd1306_fill_rectangle(dev, padding, 20, padding+4, bar_size +20, 1);
    sprintf(str, "g");
    ssd1306_draw_string(dev, padding, 52, (unsigned char *)str, 12, true);

    padding += 24;
    bar_size = lights->light[light_selected].b / 1;
    ssd1306_fill_rectangle(dev, padding, 20, padding+4, bar_size +20, 1);
    sprintf(str, "b");
    ssd1306_draw_string(dev, padding, 52, (unsigned char *)str, 12, true);

    padding += 24;
    bar_size = lights->light[light_selected].w / 1;
    ssd1306_fill_rectangle(dev, padding, 20, padding+4, bar_size +20, 1);
    sprintf(str, "w");
    ssd1306_draw_string(dev, padding, 52, (unsigned char *)str, 12, true);

    padding += 24;
    bar_size = lights->light[light_selected].y / 1;
    ssd1306_fill_rectangle(dev, padding, 20, padding+4, bar_size +20, 1);
    sprintf(str, "y");
    ssd1306_draw_string(dev, padding, 52, (unsigned char *)str, 12, true);

    ssd1306_refresh_gram(dev);
}