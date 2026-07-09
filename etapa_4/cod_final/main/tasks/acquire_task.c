#include "tasks/acquire_task.h"

#include "flight_data.h"
#include "drivers/sensor_context.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_timer.h"

#include "esp_err.h"
#include "esp_log.h"

static const char *TAG = "TASK";

flight_data_t g_flight_data;

SemaphoreHandle_t g_flight_data_mutex = NULL;

TaskHandle_t acquireTaskHandle = NULL;

static void read_barometer(void)
{
    g_flight_data.pressure =
        bmp280_get_pressure(&sensors.bmp280);

    g_flight_data.altitude =
        bmp280_get_altitude(&sensors.bmp280);
}

static void read_accelerometer(void)
{
    g_flight_data.acceleration =
        mpu6050_get_longitudinal_acceleration(
            &sensors.mpu6050);
}

static void update_kalman(void)
{
    kalman_update(
        &sensors.kalman,
        g_flight_data.altitude,
        g_flight_data.acceleration,
        esp_timer_get_time() / 1000);

    g_flight_data.filtered_altitude =
        sensors.kalman.altitude;

    g_flight_data.velocity =
        sensors.kalman.velocity;
}

static void acquire_task(void *arg)
{
    for (;;)
    {
        xSemaphoreTake(
            g_flight_data_mutex,
                portMAX_DELAY);

        read_barometer();

        read_accelerometer();

        update_kalman();

        xSemaphoreGive(
            g_flight_data_mutex);

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void acquire_task_start(void)
{

    xTaskCreatePinnedToCore(
        acquire_task,
        "Acquire",
        4096,
        NULL,
        5,
        &acquireTaskHandle,
        1);

    ESP_LOGI(TAG, "ACQUIRE TASK INICIALIZADA");
}

void acquire_task_delete(void)
{
    if (acquireTaskHandle != NULL)
    {
        vTaskDelete(acquireTaskHandle);
        acquireTaskHandle = NULL;
        ESP_LOGI(TAG, "ACQUIRE TASK TERMINADA");
    }
}