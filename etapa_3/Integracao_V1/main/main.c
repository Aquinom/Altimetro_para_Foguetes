#include "buzzer.h"
#include "faketasks.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "src-gen/Statechart.h"

//====================================================
// GLOBAL STATECHART HANDLE
//====================================================

Statechart handle;

//====================================================
// STATECHART TASK
//====================================================

void statechart_task(void *pv)
{
    while (1)
    {
        statechart_run_cycle(&handle);

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

//====================================================
// APP MAIN
//====================================================

void app_main(void)
{
    //================================================
    // INITIALIZE STATECHART
    //================================================

    statechart_init(&handle);

    statechart_enter(&handle);

    //================================================
    // CREATE TASKS
    //================================================

    xTaskCreate(
        statechart_task,
        "statechart_task",
        4096,
        NULL,
        3,
        NULL
    );

    xTaskCreate(
        accelerometer_task,
        "accelerometer_task",
        4096,
        NULL,
        2,
        NULL
    );

    xTaskCreate(
        barometer_task,
        "barometer_task",
        4096,
        NULL,
        2,
        NULL
    );

    xTaskCreate(
        peak_detection_task,
        "peak_detection_task",
        4096,
        NULL,
        2,
        NULL
    );
}