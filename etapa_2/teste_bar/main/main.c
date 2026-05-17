#include "drivers/i2c/i2c_helper.h"
#include "drivers/bmp280/bmp280.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "MAIN";

void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());

    bmp280_t bmp;

    if (!bmp280_init(&bmp, 0x76))
    {
        ESP_LOGE(TAG, "Erro ao iniciar BMP280");
        return;
    }

    while (1)
    {
        float pressure = bmp280_get_pressure(&bmp);
        float altitude = bmp280_get_altitude(&bmp);

        ESP_LOGI(
            TAG,
            "Press: %.2f Pa | Alt: %.3f m",
            pressure,
            altitude
        );

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}