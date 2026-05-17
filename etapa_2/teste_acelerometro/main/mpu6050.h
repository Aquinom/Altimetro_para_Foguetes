#pragma once

#include <stdint.h>

typedef struct
{
    float ax;
    float ay;
    float az;
} mpu6050_data_t;

void mpu6050_init(void);
void mpu6050_read_accel(mpu6050_data_t *data);