#include "flight_controller.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

#include "esp_log.h"
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "i2c_helper.h"
#include "bmp280.h"
#include "mpu6050.h"

#define SENSOR_PERIOD_MS 20
#define TELEMETRY_PERIOD_MS 200

#define MOVING_AVG_SIZE 15

#define APOGEE_MIN_ALTITUDE 0.15f
#define APOGEE_NEG_VEL_LIMIT -0.03f

static const char *TAG = "FLIGHT";

/* =====================================================
   Média móvel
===================================================== */

typedef struct
{
    float buffer[MOVING_AVG_SIZE];
    int index;
    int count;
    float sum;

} moving_avg_t;

static void ma_init(moving_avg_t *ma)
{
    memset(ma, 0, sizeof(moving_avg_t));
}

static float ma_update(moving_avg_t *ma, float value)
{
    ma->sum -= ma->buffer[ma->index];

    ma->buffer[ma->index] = value;

    ma->sum += value;

    ma->index =
        (ma->index + 1) % MOVING_AVG_SIZE;

    if (ma->count < MOVING_AVG_SIZE)
        ma->count++;

    return ma->sum / ma->count;
}


typedef struct
{
    float altitude;
    float pressure;
    float acceleration;
    float velocity;
    float max_altitude;

    bool apogee_detected;

} telemetry_t;

static telemetry_t telemetry;

static SemaphoreHandle_t telemetry_mutex;

/* =====================================================
   Task leitura dos sensores
===================================================== */

static void sensor_task(void *arg)
{
    bmp280_t bmp;
    mpu6050_t mpu;

    if (i2c_master_init() != ESP_OK)
    {
        ESP_LOGE(TAG, "Erro ao iniciar I2C");
        vTaskDelete(NULL);
    }

    if (!bmp280_init(&bmp, 0x76))
    {
        ESP_LOGE(TAG, "BMP280 nao encontrado");
        vTaskDelete(NULL);
    }

    if (!mpu6050_init(&mpu))
    {
        ESP_LOGE(TAG, "MPU6050 nao encontrado");
        vTaskDelete(NULL);
    }

    ESP_LOGI(TAG, "Calibrando sensores...");

    bmp280_calibrate_ground(&bmp);
    mpu6050_calibrate(&mpu);

    ESP_LOGI(TAG, "Calibracao concluida");

    moving_avg_t alt_filter;
    moving_avg_t accel_filter;
    moving_avg_t vel_filter;

    ma_init(&alt_filter);
    ma_init(&accel_filter);
    ma_init(&vel_filter);

    float prev_altitude = 0;
    float velocity = 0;
    float max_altitude = 0;

    bool apogee_detected = false;

    const float dt = SENSOR_PERIOD_MS / 1000.0f;

    while (1)
    {
        float pressure = bmp280_get_pressure(&bmp);

        float altitude_raw = bmp280_get_altitude(&bmp);
        float altitude = ma_update(&alt_filter, altitude_raw);

        float ax, ay, az;

        mpu6050_read_accel(&mpu, &ax, &ay, &az);

        float accel = ma_update(&accel_filter, az);

        float vel_baro = (altitude - prev_altitude) / dt;

        velocity += accel * dt * 0.25f;

        velocity =
            (velocity * 0.35f) +
            (vel_baro * 0.65f);

        velocity = ma_update(&vel_filter, velocity);

        if (fabsf(velocity) < 0.01f)
            velocity = 0;

        if (altitude > max_altitude)
            max_altitude = altitude;

        if (!apogee_detected)
        {
            bool altitude_valid =
                altitude > APOGEE_MIN_ALTITUDE;

            bool descending =
                velocity < APOGEE_NEG_VEL_LIMIT;

            bool accel_negative =
                accel < -0.02f;

            if (
                altitude_valid &&
                descending &&
                accel_negative)
            {
                apogee_detected = true;

                ESP_LOGW(TAG, "APOGEU DETECTADO");
            }
        }

        prev_altitude = altitude;

        xSemaphoreTake(telemetry_mutex, portMAX_DELAY);

        telemetry.altitude = altitude;
        telemetry.pressure = pressure;
        telemetry.acceleration = accel;
        telemetry.velocity = velocity;
        telemetry.max_altitude = max_altitude;
        telemetry.apogee_detected = apogee_detected;

        xSemaphoreGive(telemetry_mutex);

        vTaskDelay(pdMS_TO_TICKS(SENSOR_PERIOD_MS));
    }
}

/* =====================================================
   Task de telemtria
===================================================== */

static void telemetry_task(void *arg)
{
    telemetry_t local;

    while (1)
    {
        xSemaphoreTake(telemetry_mutex, portMAX_DELAY);

        local = telemetry;

        xSemaphoreGive(telemetry_mutex);

        ESP_LOGI(
            TAG,
            "ALT: %.2f m | VEL: %.2f m/s | ACC: %.3f g | PRESS: %.2f Pa | MAX: %.2f m | APOGEE: %s",
            local.altitude,
            local.velocity,
            local.acceleration,
            local.pressure,
            local.max_altitude,
            local.apogee_detected ? "SIM" : "NAO");

        vTaskDelay(pdMS_TO_TICKS(TELEMETRY_PERIOD_MS));
    }
}

/* =====================================================
   Task de inicialização de controle de voo
===================================================== */

void flight_controller_init(void)
{
    telemetry_mutex = xSemaphoreCreateMutex();

    xTaskCreatePinnedToCore(
        sensor_task,
        "sensor_task",
        4096,
        NULL,
        5,
        NULL,
        1);

    xTaskCreatePinnedToCore(
        telemetry_task,
        "telemetry_task",
        4096,
        NULL,
        1,
        NULL,
        0);
}