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
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc->adc_handle, ADC_CHANNEL_6, &config));

    return adc;
}

int get_batt_voltage(struct adc_t *adc) {

    int val, i;

    adc_oneshot_read(adc->adc_handle, ADC_CHANNEL_6, &val);

    //printf("the raw value adc : %d\n", val);

    for (i=9;i>0;i--)
        adc->batt_v_tab[i] = adc->batt_v_tab[i-1];
    adc->batt_v_tab[0] = val;

    val = 0;
    for (i=0;i<10;i++) {
        val = adc->batt_v_tab[i] + val;
    }
    val /= 10;
    adc->batt_v = (float)val * 3.61 /4096.f *2.f;
    adc->percent = battery_percent_from_voltage(adc->batt_v);

    return adc->batt_v;
}

int battery_percent_from_voltage(float volt)
{
    float v = volt;

    if (v >= 4.20f) return 100;
    else if (v > 4.10f) return 90 + (v - 4.10f) * 100;
    else if (v > 3.98f) return 80 + (v - 3.98f) * (10.0f / (4.10f - 3.98f));
    else if (v > 3.85f) return 70 + (v - 3.85f) * (10.0f / (3.98f - 3.85f));
    else if (v > 3.80f) return 60 + (v - 3.80f) * (10.0f / (3.85f - 3.80f));
    else if (v > 3.75f) return 50 + (v - 3.75f) * (10.0f / (3.80f - 3.75f));
    else if (v > 3.70f) return 40 + (v - 3.70f) * (10.0f / (3.75f - 3.70f));
    else if (v > 3.65f) return 30 + (v - 3.65f) * (10.0f / (3.70f - 3.65f));
    else if (v > 3.55f) return 20 + (v - 3.55f) * (10.0f / (3.65f - 3.55f));
    else if (v > 3.45f) return 10 + (v - 3.45f) * (10.0f / (3.55f - 3.45f));
    else if (v > 3.30f) return (v - 3.30f) * (10.0f / (3.45f - 3.30f));
    else return 0;
}
