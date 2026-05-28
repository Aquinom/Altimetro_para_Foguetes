#include "i2c_helper.h"

#define FREQ 400000

esp_err_t i2c_master_init(
    i2c_port_t port,
    gpio_num_t sda,
    gpio_num_t scl
)
{
    i2c_config_t conf =
    {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = FREQ
    };

    ESP_ERROR_CHECK(
        i2c_param_config(
            port,
            &conf
        )
    );

    return i2c_driver_install(
        port,
        conf.mode,
        0,
        0,
        0
    );
}

esp_err_t i2c_write(
    i2c_port_t port,
    uint8_t addr,
    uint8_t reg,
    uint8_t data
)
{
    uint8_t buf[2]={reg,data};

    return i2c_master_write_to_device(
        port,
        addr,
        buf,
        2,
        pdMS_TO_TICKS(100)
    );
}

esp_err_t i2c_read(
    i2c_port_t port,
    uint8_t addr,
    uint8_t reg,
    uint8_t *data,
    size_t len
)
{
    return i2c_master_write_read_device(
        port,
        addr,
        &reg,
        1,
        data,
        len,
        pdMS_TO_TICKS(100)
    );
}