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

#define DIV_ADAPT 3

void dislay_rgb_ctrl(ssd1306_handle_t dev, int light_selected, struct lights_t *lights) {
    
    char str[50];

    int padding = 0;
    int bar_size;

    ssd1306_clear_screen(dev, false);

    sprintf(str, "Light : %d", light_selected);
    ssd1306_draw_string(dev, 0, 0, (unsigned char *)str, 12, true);

    ssd1306_draw_line(dev, 0, 13, 128, 12);

    padding = 10;
    bar_size = lights->light[light_selected].r / DIV_ADAPT;

    //printf("bar size r -- %d: %d\n", light_selected, lights->light[light_selected].r);

    ssd1306_fill_rectangle(dev, padding, 51-bar_size, padding+4, 51, 1);
    sprintf(str, "r");
    ssd1306_draw_string(dev, padding, 52, (unsigned char *)str, 12, true);

    padding += 24;
    bar_size = lights->light[light_selected].g / DIV_ADAPT;
    ssd1306_fill_rectangle(dev, padding, 51-bar_size, padding+4, 51, 1);
    sprintf(str, "g");
    ssd1306_draw_string(dev, padding, 52, (unsigned char *)str, 12, true);

    padding += 24;
    bar_size = lights->light[light_selected].b / DIV_ADAPT;
    ssd1306_fill_rectangle(dev, padding, 51-bar_size, padding+4, 51, 1);
    sprintf(str, "b");
    ssd1306_draw_string(dev, padding, 52, (unsigned char *)str, 12, true);

    padding += 24;
    bar_size = lights->light[light_selected].w / DIV_ADAPT;
    ssd1306_fill_rectangle(dev, padding, 51-bar_size, padding+4, 51, 1);
    sprintf(str, "w");
    ssd1306_draw_string(dev, padding, 52, (unsigned char *)str, 12, true);

    padding += 24;
    bar_size = lights->light[light_selected].y / DIV_ADAPT;
    ssd1306_fill_rectangle(dev, padding, 51-bar_size, padding+4, 51, 1);
    sprintf(str, "y");
    ssd1306_draw_string(dev, padding, 52, (unsigned char *)str, 12, true);

    ssd1306_refresh_gram(dev);
}

void dislay_devices(ssd1306_handle_t dev, struct server_t *server) {

    char str[50];

    int is_here;
    int i;
    int x, y;

    ssd1306_clear_screen(dev, false);

    x = 0;
    y = 0;
    for (i=0;i<20;i++) {

        if (server->clients_last_seen[i] < 1000)
            is_here = 1;
        else
            is_here = 0;

        sprintf(str, "%d:%d", i, is_here);
        ssd1306_draw_string(dev, x, y, (unsigned char *)str, 12, true);

        y += 13;
        if (y > (13*4)) {
            y = 0;
            x += 34;
        }
    }

    ssd1306_refresh_gram(dev);
    
}