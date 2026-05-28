#include "buzzer.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "string.h"

//====================================================
// BUZZER INIT
//====================================================

void buzzer_init(gpio_num_t pin)
{
    gpio_config_t config = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&config);

    // Estado inicial desligado
    gpio_set_level(pin, 1);
}

//====================================================
// SINGLE BIP
//====================================================

void buzzer_1bip(gpio_num_t pin)
{
    // Liga buzzer
    gpio_set_level(pin, 0);

    vTaskDelay(pdMS_TO_TICKS(200));

    // Desliga buzzer
    gpio_set_level(pin, 1);
}

//====================================================
// READY SOUND
//====================================================

void buzzer_doneInit(gpio_num_t pin)
{
    for (int i = 0; i < 3; i++)
    {
        gpio_set_level(pin, 0);

        vTaskDelay(pdMS_TO_TICKS(200));

        gpio_set_level(pin, 1);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

//====================================================
// TEST FUNCTIONS - UM BEEP POR ESTADO
//====================================================

void buzzer_stateBeep(gpio_num_t pin, const char *state)
{
    int i = 0;

    if (strcmp(state, "PRELAUNCH") == 0)
    {
        i = 1;
    }
    else if (strcmp(state, "LAUNCH") == 0)
    {
        i = 2;
    }
    else if (strcmp(state, "FLYING") == 0)
    {
        i = 3;
    }
    else if (strcmp(state, "PEAK") == 0)
    {
        i = 4;
    }
    else if (strcmp(state, "DROPPING") == 0)
    {
        i = 5;
    }
    else if (strcmp(state, "FINDME") == 0)
    {
        i = 6;
    }
    else
    {
        i = 0;
    }

    for (int index = 0; index < i; index++)
    {
        gpio_set_level(pin, 0);

        vTaskDelay(pdMS_TO_TICKS(200));

        gpio_set_level(pin, 1);

        vTaskDelay(pdMS_TO_TICKS(200));
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
}