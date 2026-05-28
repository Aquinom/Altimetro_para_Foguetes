#include "src-gen/statechart_required.h"

#include "buzzer.h"
#include "faketasks.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

static const char *TAG = "UART";

//====================================================
// UART OPERATIONS
//====================================================

void statechart_sm_initUart(const Statechart* handle, const sc_integer baudrate)
{
    ESP_LOGI(TAG, "UART initialized at %ld baud", (long)baudrate);
}

void statechart_sm_printState(const Statechart* handle, const sc_string state)
{
    ESP_LOGI(TAG, "Current State: %s", state);
}

//====================================================
// BUZZER OPERATIONS
//====================================================

void statechart_sm_buzzerInit(const Statechart* handle, const sc_integer pin)
{
    buzzer_init((gpio_num_t)pin);

    ESP_LOGI(TAG, "Buzzer initialized on GPIO %ld", (long)pin);
}

void statechart_sm_buzzerFindMe(const Statechart* handle, const sc_integer pin)
{
    buzzer_1bip((gpio_num_t)pin);

    ESP_LOGI(TAG, "FindMe bip");
}

void statechart_sm_bipReady(const Statechart* handle, const sc_integer pin)
{
    buzzer_doneInit((gpio_num_t)pin);

    ESP_LOGI(TAG, "Ready bip executed");
}

void statechart_sm_stateBeep(
    const Statechart* handle,
    const sc_integer pin,
    const sc_string state
)
{
    ESP_LOGI(TAG, "Beep State: %s starting", state);

    buzzer_stateBeep((gpio_num_t)pin, (const char*)state);

    ESP_LOGI(TAG, "Beep State: %s complete", state);
}

//====================================================
// CONTROL OPERATIONS
//====================================================

void statechart_sm_enableSensors(const Statechart* handle)
{
    accelerometer_enabled = true;
    barometer_enabled = true;

    ESP_LOGI(
        TAG,
        "Sensors ENABLED"
    );
}

void statechart_sm_disableSensors(const Statechart* handle)
{
    accelerometer_enabled = false;
    barometer_enabled = false;

    ESP_LOGI(
        TAG,
        "Sensors DISABLED"
    );
}