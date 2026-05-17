#pragma once

#include "esp_err.h"
#include <stdint.h>
#include <stddef.h>

esp_err_t i2c_master_init(void);

esp_err_t i2c_write(
    uint8_t addr,
    uint8_t reg,
    uint8_t data
);

esp_err_t i2c_read(
    uint8_t addr,
    uint8_t reg,
    uint8_t *data,
    size_t len
);