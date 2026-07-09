#pragma once

#include "bmp280.h"
#include "mpu6050.h"
#include "filter/kalman_altitude.h"

typedef struct
{
    bmp280_t bmp280;
    mpu6050_t mpu6050;
    kalman_altitude_t kalman;

} sensor_context_t;

extern sensor_context_t sensors;