#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "driver/i2c.h"

typedef struct
{
    uint16_t T1;
    int16_t T2;
    int16_t T3;

    uint16_t P1;
    int16_t P2;
    int16_t P3;
    int16_t P4;
    int16_t P5;
    int16_t P6;
    int16_t P7;
    int16_t P8;
    int16_t P9;

} bmp280_calib_data_t;

typedef struct
{
    i2c_port_t port;

    uint8_t address;

    bmp280_calib_data_t calib;

    int32_t t_fine;

    float base_pressure;

} bmp280_t;

bool bmp280_init(
    bmp280_t *dev,
    i2c_port_t port,
    uint8_t addr);

float bmp280_get_pressure(
    bmp280_t *dev);

void bmp280_calibrate_ground(
    bmp280_t *dev);

float bmp280_get_altitude(
    bmp280_t *dev);