#include "oled.h"

ssd1306_handle_t oled_init() {

i2c_master_bus_config_t i2c_mst_config_1 = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_PORT,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };


    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x3C,
        .scl_speed_hz = 400000,
    };

    i2c_master_bus_handle_t *bus_handle = (i2c_master_bus_handle_t *) malloc(sizeof(i2c_master_bus_handle_t));
    i2c_master_dev_handle_t *dev_handle = (i2c_master_dev_handle_t *) malloc(sizeof(i2c_master_dev_handle_t));

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config_1, bus_handle));
    ESP_ERROR_CHECK(i2c_master_probe(*bus_handle, 0x3c, -1));


    i2c_master_bus_add_device(*bus_handle, &dev_cfg, dev_handle);

    ssd1306_handle_t dev = ssd1306_create(*dev_handle);
    if (dev == NULL) {
        printf("Init error : SSD1306\n");
    }

    return dev;
}