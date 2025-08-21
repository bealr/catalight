#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <stdint.h>
#include "oled.h"
#include "defines.h"

void dislay_welcome(ssd1306_handle_t dev);
void dislay_light_select(ssd1306_handle_t dev, int *light_selected, int force_refresh);
void dislay_rgb_ctrl(ssd1306_handle_t dev, int light_selected, struct lights_t *lights);

#endif