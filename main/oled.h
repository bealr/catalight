#ifndef __OLED_H
#define __OLED_H

#include <driver/i2c_master.h>
#include "ssd1306.h"

#define I2C_MASTER_SCL_IO   22
#define I2C_MASTER_SDA_IO   21
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000
#define I2C_MASTER_PORT     0

ssd1306_handle_t oled_init();

#endif