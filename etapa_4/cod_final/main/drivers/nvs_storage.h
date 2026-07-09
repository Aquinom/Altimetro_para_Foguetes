#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"

#define NVS_MAX_FLIGHTS 10

esp_err_t nvs_storage_init(void);

esp_err_t nvs_save_max_altitude(float altitude);

bool nvs_get_altitude(uint8_t index, float *altitude);

uint32_t nvs_get_counter(void);

void nvs_print_history(void);