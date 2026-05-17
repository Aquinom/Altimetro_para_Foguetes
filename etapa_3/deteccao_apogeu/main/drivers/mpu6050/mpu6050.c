#include "mpu6050.h"
#include "../i2c/i2c_helper.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdio.h>
#include <math.h>

#define MPU6050_ADDR         0x68
#define MPU6050_PWR_MGMT_1   0x6B
#define MPU6050_ACCEL_XOUT_H 0x3B

#define LAUNCH_THRESHOLD_G 1.5f
#define APOGEE_THRESHOLD_G 0.10f
#define ALPHA 0.2f
#define DEAD_BAND 0.03f
#define APOGEE_COUNT 25


static int launched = 0;
static int apogee = 0;

static int stable_counter = 0;
static float accel_bias = 0.0f;


static float lowpass(float x)
{
    static float y = 0;

    y =
        ALPHA * x +
        (1 - ALPHA) * y;

    return y;
}


void mpu6050_read_accel(
    mpu6050_data_t *out
)
{
    uint8_t reg =
        MPU6050_ACCEL_XOUT_H;

    uint8_t data[6];

    i2c_read(
        MPU6050_ADDR,
        reg,
        data,
        6
    );

    int16_t ax =
        (data[0] << 8) | data[1];

    int16_t ay =
        (data[2] << 8) | data[3];

    int16_t az =
        (data[4] << 8) | data[5];

    out->ax = ax / 16384.0f;
    out->ay = ay / 16384.0f;
    out->az = az / 16384.0f;
}


static void calibrate_bias(void)
{
    mpu6050_data_t d;
    float sum = 0;

    printf("Calibrando bias...\n");

    for (int i=0; i<200; i++)
    {
        mpu6050_read_accel(&d);

        sum += d.az;

        vTaskDelay(
            pdMS_TO_TICKS(10)
        );
    }

    accel_bias =
        sum / 200.0f;

    printf(
        "Bias calibrado: %.4f g\n",
        accel_bias
    );
}


void mpu6050_init(void)
{
    i2c_write(
        MPU6050_ADDR,
        MPU6050_PWR_MGMT_1,
        0x00
    );

    vTaskDelay(
        pdMS_TO_TICKS(100)
    );

    calibrate_bias();
}


void mpu6050_update(void)
{
    static TickType_t last_print = 0;

    mpu6050_data_t d;

    mpu6050_read_accel(&d);

    float az =
        d.az - accel_bias;

    if (fabs(az) < DEAD_BAND)
        az = 0;

    az =
        lowpass(az);

    if (!launched &&
        fabs(az) > LAUNCH_THRESHOLD_G)
    {
        launched = 1;

        printf(
            "LANCAMENTO DETECTADO\n"
        );
    }

    if (launched && !apogee)
    {
        if (fabs(az) < APOGEE_THRESHOLD_G)
            stable_counter++;
        else
            stable_counter = 0;

        if (stable_counter >= APOGEE_COUNT)
        {
            apogee = 1;

            printf(
                "APOGEU DETECTADO\n"
            );
        }
    }

    TickType_t now =
        xTaskGetTickCount();

    if ((now - last_print) >
        pdMS_TO_TICKS(1000))
    {
        printf(
            "Acc: %.2f g | Apo: %d\n",
            az,
            apogee
        );

        last_print = now;
    }

    vTaskDelay(
        pdMS_TO_TICKS(20)
    );
}


int mpu6050_apogee_detected(void)
{
    return apogee;
}