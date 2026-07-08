#include "buzzer.h"

#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"
#include <stdbool.h>

#define BUZZER_PIN GPIO_NUM_12

static const char *TAG = "BUZZER";

static bool initialized = false;

static void buzzer_configure(void)
{
    if (initialized)
    {
        return;
    }

    gpio_config_t config =
    {
        .pin_bit_mask = (1ULL << BUZZER_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&config);

    gpio_set_level(BUZZER_PIN, 1);

    initialized = true;
}

void buzzer_init(void)
{
    buzzer_configure();
}

void buzzer_beep_ms(uint32_t duration_ms)
{
    ESP_LOGI(TAG, "BEEPING");

    buzzer_configure();

    gpio_set_level(BUZZER_PIN, 0);

    vTaskDelay(pdMS_TO_TICKS(duration_ms));

    gpio_set_level(BUZZER_PIN, 1);
}

void buzzer_DoneInit(void)
{
    buzzer_configure();

    for (int i = 0; i < 3; i++)
    {
        gpio_set_level(BUZZER_PIN, 0);

        vTaskDelay(pdMS_TO_TICKS(200));

        gpio_set_level(BUZZER_PIN, 1);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}