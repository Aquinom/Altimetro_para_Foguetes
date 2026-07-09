#include "tasks/flight_task.h"

#include "flight_data.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "drivers/nvs_storage.h"

#include "esp_log.h"

extern flight_data_t g_flight_data;
extern SemaphoreHandle_t g_flight_data_mutex;

TaskHandle_t flightTaskHandle = NULL;

flight_status_t g_flight_status = {0};

#define PEAK_MIN_DESCENT_SPEED     (-0.5) //-1.0f
#define PEAK_CONFIRMATION_COUNT    5 //20

static const char *TAG = "TASK";

typedef struct
{
    uint32_t peak_counter;

} flight_task_context_t;

static flight_task_context_t context = {0};

//static void detect_peak(void)
static void detect_peak(const flight_data_t *data)
{
    if (data->filtered_altitude >
        g_flight_status.peak_altitude)
    {
        g_flight_status.peak_altitude =
            data->filtered_altitude;
    }

    if (data->velocity < PEAK_MIN_DESCENT_SPEED)
    {
        context.peak_counter++;

        if (context.peak_counter >= PEAK_CONFIRMATION_COUNT)
        {
            g_flight_status.peak_detected = true;

            esp_err_t err =
                    nvs_save_max_altitude(
                        g_flight_status.peak_altitude);

            if (err != ESP_OK)
            {
                ESP_LOGE(TAG, "Falha ao salvar altitude: %s",
                        esp_err_to_name(err));
            }

            ESP_LOGI(TAG,
                     "Peak detected at %.2f m",
                     g_flight_status.peak_altitude);
        }
    }
    else
    {
        context.peak_counter = 0;
    }
}

static void update_flight_status(void)
{

    flight_data_t data;

    xSemaphoreTake(
        g_flight_data_mutex,
        portMAX_DELAY);

    data = g_flight_data;

    xSemaphoreGive(
        g_flight_data_mutex);

    if (!g_flight_status.peak_detected)
    {
        detect_peak(&data);;
    }
}

static void flight_task(void *arg)
{
    for (;;)
    {
        update_flight_status();

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void flight_task_start(void)
{
    xTaskCreatePinnedToCore(
        flight_task,
        "Flight",
        4096,
        NULL,
        4,
        &flightTaskHandle,
        1);

    ESP_LOGI(TAG, "FLIGHT TASK INICIALIZADA");
}

bool flight_peak_detected(void)
{
    return g_flight_status.peak_detected;
}

float flight_peak_altitude(void)
{
    return g_flight_status.peak_altitude;
}

void flight_task_delete(void)
{
    if (flightTaskHandle != NULL)
    {
        vTaskDelete(flightTaskHandle);
        flightTaskHandle = NULL;
        ESP_LOGI(TAG, "FLIGHT TASK TERMINADA");
    }
}