#include "buzzer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    buzzer_1bip(GPIO_NUM_18);

    vTaskDelay(pdMS_TO_TICKS(2000));

    buzzer_DoneInit(GPIO_NUM_18);
}