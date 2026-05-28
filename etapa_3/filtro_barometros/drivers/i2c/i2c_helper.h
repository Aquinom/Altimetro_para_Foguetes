#pragma once

#include "esp_err.h"
#include "driver/i2c.h"

#include <stdint.h>
#include <stddef.h>

esp_err_t i2c_master_init(
    i2c_port_t port,
    gpio_num_t sda,
    gpio_num_t scl
);

esp_err_t i2c_write(
    i2c_port_t port,
    uint8_t addr,
    uint8_t reg,
    uint8_t data
);

esp_err_t i2c_read(
    i2c_port_t port,
    uint8_t addr,
    uint8_t reg,
    uint8_t *data,
    size_t len
);