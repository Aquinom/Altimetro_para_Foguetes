#pragma once

#include <stdbool.h>

#include "driver/i2c.h"

typedef struct
{
    i2c_port_t port;

    float ax;
    float ay;
    float az;

    float longitudinal_acceleration;

    float offset_x;
    float offset_y;
    float offset_z;

} mpu6050_t;

bool mpu6050_init(
    mpu6050_t *dev,
    i2c_port_t port);

void mpu6050_calibrate(
    mpu6050_t *dev);

bool mpu6050_read_accel(
    mpu6050_t *dev,
    float *ax,
    float *ay,
    float *az);

float mpu6050_get_longitudinal_acceleration(
    mpu6050_t *dev);