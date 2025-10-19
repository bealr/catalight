#include "adc.h"

struct adc_t *adc_init() {

    struct adc_t *adc = (struct adc_t *) malloc(sizeof(struct adc_t));

    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,  // ou ADC_UNIT_2
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc->adc_handle));

    // 2️⃣ Configurer le canal
    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_12,     // 0–3.3V environ
        .bitwidth = ADC_BITWIDTH_12,  // 12 bits
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc->adc_handle, ADC_CHANNEL_5, &config));

    return adc;
}

int get_batt_voltage(struct adc_t *adc) {

    int val;

    adc_oneshot_read(adc->adc_handle, ADC_CHANNEL_5, &val);

    adc->batt_v = val;

    return adc->batt_v;
}