#ifndef __OLED_H
#define __OLED_H

#include "driver/i2c.h"
#include "ssd1306.h"

#define I2C_MASTER_SDA_IO 33
#define I2C_MASTER_SCL_IO 35
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 400000

ssd1306_handle_t oled_init();

#endif