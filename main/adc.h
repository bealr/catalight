#ifndef __ADC_H
#define __ADC_H

#include "esp_adc/adc_oneshot.h"

struct adc_t {

    adc_oneshot_unit_handle_t adc_handle;
    int batt_v_tab[10];
    float batt_v;
    int percent;
};

struct adc_t *adc_init();
int get_batt_voltage(struct adc_t *adc);
int battery_percent_from_voltage(float volt);

#endif