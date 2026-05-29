#pragma once

#include <stdbool.h>

typedef struct
{
    float ax;
    float ay;
    float az;

    float offset_x;
    float offset_y;
    float offset_z;

} mpu6050_t;

bool mpu6050_init(mpu6050_t *dev);

bool mpu6050_read_accel(
    mpu6050_t *dev,
    float *ax,
    float *ay,
    float *az);

void mpu6050_calibrate(mpu6050_t *dev);