#include "mpu6050.h"
#include "i2c_helper.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#define MPU6050_ADDR          0x68

#define MPU6050_PWR_MGMT_1    0x6B
#define MPU6050_ACCEL_XOUT_H  0x3B
#define MPU6050_WHO_AM_I      0x75

#define MPU6050_ACCEL_SCALE   16384.0f
#define GRAVITY               9.80665f

static const char *TAG = "MPU6050";

static bool read_raw_accel(
    mpu6050_t *dev,
    int16_t *ax,
    int16_t *ay,
    int16_t *az)
{
    uint8_t data[6];

    if (i2c_read_port(
            dev->port,
            MPU6050_ADDR,
            MPU6050_ACCEL_XOUT_H,
            data,
            6) != ESP_OK)
    {
        return false;
    }

    *ax = ((int16_t)data[0] << 8) | data[1];
    *ay = ((int16_t)data[2] << 8) | data[3];
    *az = ((int16_t)data[4] << 8) | data[5];

    return true;
}

bool mpu6050_init(
    mpu6050_t *dev,
    i2c_port_t port)
{
    uint8_t whoami;

    dev->port = port;

    if (i2c_read_port(
            port,
            MPU6050_ADDR,
            MPU6050_WHO_AM_I,
            &whoami,
            1) != ESP_OK)
    {
        return false;
    }

    if (whoami != 0x68)
        return false;

    if (i2c_write_port(
            port,
            MPU6050_ADDR,
            MPU6050_PWR_MGMT_1,
            0x00) != ESP_OK)
    {
        return false;
    }

    vTaskDelay(pdMS_TO_TICKS(100));

    dev->offset_x = 0;
    dev->offset_y = 0;
    dev->offset_z = 0;

    dev->ax = 0;
    dev->ay = 0;
    dev->az = 0;

    dev->longitudinal_acceleration = 0;

    ESP_LOGI(TAG, "MPU6050 inicializado");

    return true;
}

void mpu6050_calibrate(
    mpu6050_t *dev)
{
    const int samples = 200;

    float sx = 0;
    float sy = 0;
    float sz = 0;

    for (int i = 0; i < samples; i++)
    {
        int16_t ax, ay, az;

        if (read_raw_accel(
                dev,
                &ax,
                &ay,
                &az))
        {
            sx += ax / MPU6050_ACCEL_SCALE;
            sy += ay / MPU6050_ACCEL_SCALE;
            sz += (az / MPU6050_ACCEL_SCALE) - 1.0f;
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
    int16_t raw_ax;
    int16_t raw_ay;
    int16_t raw_az;

    if (!read_raw_accel(
            dev,
            &raw_ax,
            &raw_ay,
            &raw_az))
    {
        return false;
    }

    *ax =
        ((raw_ax / MPU6050_ACCEL_SCALE) -
         dev->offset_x) *
        GRAVITY;

    *ay =
        ((raw_ay / MPU6050_ACCEL_SCALE) -
         dev->offset_y) *
        GRAVITY;

    *az =
        (((raw_az / MPU6050_ACCEL_SCALE) - 1.0f) -
         dev->offset_z) *
        GRAVITY;

    dev->ax = *ax;
    dev->ay = *ay;
    dev->az = *az;

    /*
     * Eixo longitudinal do foguete
     * MPU alinhado com o tubo.
     */
    dev->longitudinal_acceleration = *az;

    return true;
}

float mpu6050_get_longitudinal_acceleration(
    mpu6050_t *dev)
{
    return dev->longitudinal_acceleration;
}