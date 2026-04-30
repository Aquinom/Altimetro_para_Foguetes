#include "buzzer.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void buzzer_1bip(gpio_num_t pin)
{
    gpio_config_t config = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&config);

    // Liga o buzzer
    gpio_set_level(pin, 0);
    vTaskDelay(pdMS_TO_TICKS(200));

    // Desliga
    gpio_set_level(pin, 1);
}

void buzzer_DoneInit(gpio_num_t pin)
{
    gpio_config_t config = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&config);

    for (int i = 0; i < 3; i++)
    {
        gpio_set_level(pin, 0);
        vTaskDelay(pdMS_TO_TICKS(200));

        gpio_set_level(pin, 1);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}