#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <stdint.h>
#include "oled.h"
#include "defines.h"
#include "server.h"

void dislay_welcome(ssd1306_handle_t dev, int force_refresh);
void dislay_light_select(ssd1306_handle_t dev, int *light_selected, struct lights_t *lights, int force_refresh);
void dislay_rgb_ctrl(ssd1306_handle_t dev, int light_selected, struct lights_t *lights);
void dislay_devices(ssd1306_handle_t dev, struct lights_t *lights);

#endif