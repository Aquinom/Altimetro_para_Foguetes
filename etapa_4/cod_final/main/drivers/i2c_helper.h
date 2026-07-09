#pragma once

#include "driver/i2c.h"
#include "esp_err.h"

// #define I2C0_SDA GPIO_NUM_21
// #define I2C0_SCL GPIO_NUM_22

// #define I2C1_SDA GPIO_NUM_25
// #define I2C1_SCL GPIO_NUM_26

#define I2C0_SDA GPIO_NUM_25
#define I2C0_SCL GPIO_NUM_26

#define I2C1_SDA GPIO_NUM_18
#define I2C1_SCL GPIO_NUM_19

#define I2C_FREQ_HZ 400000

esp_err_t i2c_master_init_all(void);

esp_err_t i2c_write_port(
    i2c_port_t port,
    uint8_t addr,
    uint8_t reg,
    uint8_t data);

esp_err_t i2c_read_port(
    i2c_port_t port,
    uint8_t addr,
    uint8_t reg,
    uint8_t *data,
    size_t len);