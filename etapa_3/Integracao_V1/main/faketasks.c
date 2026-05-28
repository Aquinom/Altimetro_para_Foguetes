#include "faketasks.h"

#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "src-gen/Statechart.h"

//====================================================
// EXTERN STATECHART HANDLE
//====================================================

extern Statechart handle;

//====================================================
// GLOBAL VARIABLES
//====================================================

volatile bool accelerometer_enabled = false;
volatile bool barometer_enabled = false;

volatile int accelerometer_value = 0;
volatile int barometer_value = 0;

//====================================================
// TAG
//====================================================

static const char *TAG = "FAKE_TASKS";

//====================================================
// ACCELEROMETER TASK
//====================================================

void accelerometer_task(void *pv)
{
    while (1)
    {
        if (accelerometer_enabled)
        {
            accelerometer_value = rand() % 101;

            ESP_LOGI(
                TAG,
                "Accelerometer: %d",
                accelerometer_value
            );
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//====================================================
// BAROMETER TASK
//====================================================

void barometer_task(void *pv)
{
    while (1)
    {
        if (barometer_enabled)
        {
            barometer_value = rand() % 101;

            ESP_LOGI(
                TAG,
                "Barometer: %d",
                barometer_value
            );
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

//====================================================
// PEAK DETECTION TASK
//====================================================

void peak_detection_task(void *pv)
{
    while (1)
    {
        if (accelerometer_enabled &&
            barometer_enabled)
        {
            if (accelerometer_value > 90 &&
                barometer_value > 90)
            {
                ESP_LOGI(
                    TAG,
                    "PEAK DETECTED"
                );

                statechart_raise_activatePeak(&handle);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}   