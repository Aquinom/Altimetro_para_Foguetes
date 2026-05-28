#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "../bmp280/bmp280.h"

typedef struct
{
    bmp280_t bmp;

    bool enabled;

    int32_t pressure;

    int32_t ema;

    int32_t diff;

    int32_t weight;

}
baro_t;


typedef struct
{
    baro_t b1;

    baro_t b2;

    baro_t b3;

    int32_t median;

    int32_t fused_pressure;

    float altitude;

    float altitude_offset;

    float altitude_relative;

}
baro_fusion_t;


void baro_fusion_init(
    baro_fusion_t *f
);


void baro_fusion_update(
    baro_fusion_t *f
);


void baro_fusion_calibrate_zero(
    baro_fusion_t *f,
    int samples,
    int delay_ms
);