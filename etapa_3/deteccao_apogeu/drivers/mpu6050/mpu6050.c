#include "mpu6050.h"
#include "i2c_helper.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include <math.h>

#define MPU6050_ADDR 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_WHO_AM_I 0x75

static const char *TAG = "MPU6050";

static bool read_raw_accel(
    int16_t *ax,
    int16_t *ay,
    int16_t *az)
{
    uint8_t reg = MPU6050_ACCEL_XOUT_H;
    uint8_t data[6];

    if (i2c_read(MPU6050_ADDR, reg, data, 6) != ESP_OK)
        return false;

    *ax = (data[0] << 8) | data[1];
    *ay = (data[2] << 8) | data[3];
    *az = (data[4] << 8) | data[5];

    return true;
}

bool mpu6050_init(mpu6050_t *dev)
{
    uint8_t whoami;

    if (i2c_read(MPU6050_ADDR, MPU6050_WHO_AM_I, &whoami, 1) != ESP_OK)
        return false;

    if (whoami != 0x68)
        return false;

    if (i2c_write(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x00) != ESP_OK)
        return false;

    vTaskDelay(pdMS_TO_TICKS(100));

    dev->offset_x = 0;
    dev->offset_y = 0;
    dev->offset_z = 0;

    ESP_LOGI(TAG, "MPU6050 inicializado");

    return true;
}

void mpu6050_calibrate(mpu6050_t *dev)
{
    const int samples = 200;

    float sx = 0;
    float sy = 0;
    float sz = 0;

    for (int i = 0; i < samples; i++)
    {
        int16_t ax, ay, az;

        if (read_raw_accel(&ax, &ay, &az))
        {
            sx += ax / 16384.0f;
            sy += ay / 16384.0f;
            sz += (az / 16384.0f) - 1.0f;
        }

        vTaskDelay(pdMS_TO_TICKS(5));
    }

    dev->offset_x = sx / samples;
    dev->offset_y = sy / samples;
    dev->offset_z = sz / samples;

    ESP_LOGI(TAG, "Calibracao concluida");
}

bool mpu6050_read_accel(
    mpu6050_t *dev,
    float *ax,
    float *ay,
    float *az)
{
    int16_t rax, ray, raz;

    if (!read_raw_accel(&rax, &ray, &raz))
        return false;

    *ax = (rax / 16384.0f) - dev->offset_x;
    *ay = (ray / 16384.0f) - dev->offset_y;
    *az = ((raz / 16384.0f) - 1.0f) - dev->offset_z;

    return true;
}