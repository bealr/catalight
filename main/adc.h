#ifndef __ADC_H
#define __ADC_H

#include "esp_adc/adc_oneshot.h"

struct adc_t {

    adc_oneshot_unit_handle_t adc_handle;
    int batt_v;
};

struct adc_t *adc_init();
int get_batt_voltage(struct adc_t *adc);

#endif