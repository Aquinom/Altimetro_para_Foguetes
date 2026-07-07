#include "teste.h"
#include "servo.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SERVO_GPIO 18

static void servo_test_task(void *arg)
{
    while (1)
    {
        servo_set_angle(0);
        vTaskDelay(pdMS_TO_TICKS(2000));

        servo_set_angle(90);
        vTaskDelay(pdMS_TO_TICKS(2000));

        servo_set_angle(180);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void teste_start(void)
{
    servo_init(SERVO_GPIO);

    xTaskCreate(
        servo_test_task,
        "servo_test_task",
        2048,
        NULL,
        5,
        NULL
    );
}