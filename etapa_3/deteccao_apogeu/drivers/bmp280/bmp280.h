#ifndef BMP280_H
#define BMP280_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint16_t T1;
    int16_t T2, T3;

    uint16_t P1;
    int16_t P2, P3, P4, P5, P6, P7, P8, P9;

} bmp280_calib_data_t;

typedef struct
{
    uint8_t address;

    bmp280_calib_data_t calib;

    int32_t t_fine;

    float last_pressure;
    float altitude_offset;

    float base_pressure;

} bmp280_t;

bool bmp280_init(
    bmp280_t *dev,
    uint8_t addr);

float bmp280_get_pressure(
    bmp280_t *dev);

float bmp280_get_altitude(
    bmp280_t *dev);

void bmp280_calibrate_ground(
    bmp280_t *dev);

#endif