#include <stdio.h>
#include "driver/i2c_master.h"
#include "esp_err.h"

void app_main(void)
{

    i2c_master_bus_config_t i2c_mst_config_1 = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_NUM_0,
        .scl_io_num = 6,
        .sda_io_num = 5,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x76,
        .scl_speed_hz = 100000,
    };

    uint8_t buf = 0xD0;
    uint8_t buffer;

    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;

    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config_1, &bus_handle));
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
    
    esp_err_t result = i2c_master_transmit_receive(dev_handle, &buf, 1, &buffer, 1, -1);
    
    if (result == ESP_OK)
    {
        printf("The data is %02x", buffer);
    }
    else{
        printf("BAD data");
    }
    

    ESP_ERROR_CHECK(i2c_master_bus_rm_device(dev_handle));
    ESP_ERROR_CHECK(i2c_del_master_bus(bus_handle));
}