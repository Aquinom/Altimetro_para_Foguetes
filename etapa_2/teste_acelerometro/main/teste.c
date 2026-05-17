#include "teste.h"
#include "mpu6050.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void mpu_task(void *pvParameters)
{
    mpu6050_data_t data;

    while (1)
    {
        mpu6050_read_accel(&data);

        printf("AX: %.2f | AY: %.2f | AZ: %.2f\n",
               data.ax,
               data.ay,
               data.az);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void teste_start(void)
{
    mpu6050_init();

    xTaskCreate(
        mpu_task,
        "mpu_task",
        4096,
        NULL,
        5,
        NULL
    );
}