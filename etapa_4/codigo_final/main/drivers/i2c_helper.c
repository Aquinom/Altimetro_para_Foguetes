#include "i2c_helper.h"

#include "driver/i2c.h"

static esp_err_t init_bus(
    i2c_port_t port,
    gpio_num_t sda,
    gpio_num_t scl)
{
    i2c_config_t conf =
    {
        .mode = I2C_MODE_MASTER,

        .sda_io_num = sda,
        .scl_io_num = scl,

        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,

        .master.clk_speed = I2C_FREQ_HZ
    };

    ESP_ERROR_CHECK(
        i2c_param_config(
            port,
            &conf));

    return i2c_driver_install(
        port,
        conf.mode,
        0,
        0,
        0);
}

esp_err_t i2c_master_init_all(void)
{
    esp_err_t ret;

    ret =
        init_bus(
            I2C_NUM_0,
            I2C0_SDA,
            I2C0_SCL);

    if (ret != ESP_OK)
    {
        return ret;
    }

    ret =
        init_bus(
            I2C_NUM_1,
            I2C1_SDA,
            I2C1_SCL);

    return ret;
}

esp_err_t i2c_write_port(
    i2c_port_t port,
    uint8_t addr,
    uint8_t reg,
    uint8_t data)
{
    uint8_t buf[2] =
    {
        reg,
        data
    };

    return i2c_master_write_to_device(
        port,
        addr,
        buf,
        sizeof(buf),
        pdMS_TO_TICKS(1000));
}

esp_err_t i2c_read_port(
    i2c_port_t port,
    uint8_t addr,
    uint8_t reg,
    uint8_t *data,
    size_t len)
{
    return i2c_master_write_read_device(
        port,
        addr,
        &reg,
        1,
        data,
        len,
        pdMS_TO_TICKS(1000));
}