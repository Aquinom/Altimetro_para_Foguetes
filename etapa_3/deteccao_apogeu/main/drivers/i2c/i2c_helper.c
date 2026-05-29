#include "i2c_helper.h"
#include "driver/i2c.h"

#define SDA_PIN 21
#define SCL_PIN 22

#define I2C_PORT I2C_NUM_0
#define FREQ 100000

esp_err_t i2c_master_init(void)
{
    i2c_config_t conf =
        {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = SDA_PIN,
            .scl_io_num = SCL_PIN,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = FREQ};

    i2c_param_config(I2C_PORT, &conf);

    return i2c_driver_install(
        I2C_PORT,
        conf.mode,
        0,
        0,
        0);
}

esp_err_t i2c_write(
    uint8_t addr,
    uint8_t reg,
    uint8_t data)
{
    uint8_t buf[2] = {reg, data};

    return i2c_master_write_to_device(
        I2C_PORT,
        addr,
        buf,
        2,
        1000 / portTICK_PERIOD_MS);
}

esp_err_t i2c_read(
    uint8_t addr,
    uint8_t reg,
    uint8_t *data,
    size_t len)
{
    return i2c_master_write_read_device(
        I2C_PORT,
        addr,
        &reg,
        1,
        data,
        len,
        1000 / portTICK_PERIOD_MS);
}