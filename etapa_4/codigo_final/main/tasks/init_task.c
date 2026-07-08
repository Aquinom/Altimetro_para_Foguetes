#include "tasks/init_task.h"

#include "tasks/acquire_task.h"
#include "tasks/flight_task.h"

#include "drivers/i2c_helper.h"
#include "drivers/bmp280.h"
#include "drivers/mpu6050.h"
#include "drivers/sensor_context.h"
#include "drivers/servo.h"
#include "drivers/buzzer.h"

#include "filter/kalman_altitude.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "flight_data.h"

#include "esp_log.h"
#include "esp_err.h"

static const char *TAG = "INIT";

//static void init_task(void *arg)
void init_task(void)
{
    ESP_LOGI(TAG, "Initializing I2C...");

    ESP_ERROR_CHECK(i2c_master_init_all());

    ESP_LOGI(TAG, "Initializing BMP280...");

    if (!bmp280_init(
            &sensors.bmp280,
            I2C_NUM_0,
            0x76))
    {
        ESP_LOGE(TAG, "BMP280 initialization failed.");
        return;
    }

    ESP_LOGI(TAG, "Initializing MPU6050...");

    if (!mpu6050_init(
            &sensors.mpu6050,
            I2C_NUM_1))
    {
        ESP_LOGE(TAG, "MPU6050 initialization failed.");
        return;
    }

    ESP_LOGI(TAG, "Calibrating BMP280...");

    bmp280_calibrate_ground(&sensors.bmp280);

    ESP_LOGI(TAG, "Calibrating MPU6050...");

    mpu6050_calibrate(&sensors.mpu6050);

    ESP_LOGI(TAG, "Initializing Kalman filter...");

    kalman_init(&sensors.kalman);

    ESP_LOGI(TAG, "Initializing servo...");

    servo_init();

    ESP_LOGI(TAG, "Initializing buzzer...");

    buzzer_init();

    ESP_LOGI(TAG, "Creating flight data mutex...");

    g_flight_data_mutex = xSemaphoreCreateMutex();

    configASSERT(g_flight_data_mutex);

    ESP_LOGI(TAG, "Starting initialization complete beep");

    buzzer_DoneInit();

    ESP_LOGI(TAG, "Initialization complete.");

    //vTaskDelete(NULL);
}

// void init_task_start(void)
// {
//     xTaskCreatePinnedToCore(
//         init_task,
//         "Init",
//         4096,
//         NULL,
//         4,
//         NULL,
//         1);
// }