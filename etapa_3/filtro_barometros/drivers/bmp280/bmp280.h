#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "driver/i2c.h"

typedef struct
{
    uint16_t T1;
    int16_t  T2;
    int16_t  T3;

    uint16_t P1;
    int16_t  P2;
    int16_t  P3;
    int16_t  P4;
    int16_t  P5;
    int16_t  P6;
    int16_t  P7;
    int16_t  P8;
    int16_t  P9;

}
bmp280_calib_t;


typedef struct
{
    uint8_t address;

    i2c_port_t port;

    bmp280_calib_t calib;

    int32_t t_fine;

    float pressure;

}
bmp280_t;


bool bmp280_init(
    bmp280_t *dev,
    uint8_t addr
);


float bmp280_get_pressure(
    bmp280_t *dev
);


float bmp280_pressure_to_altitude(
    float pressure
);