#include "drivers/i2c/i2c_helper.h"
#include "drivers/bmp280/bmp280.h"
#include "drivers/sensor_fusion/baro_fusion.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#define BARO_UPDATE_MS 10

#define ZERO_SAMPLES   30
#define ZERO_DELAY_MS  100

static const char *TAG="MAIN";

static baro_fusion_t fusion;


/*
=========================
TASK BARO
=========================
*/
void task_baro(
    void *arg
)
{
    while(1)
    {
        baro_fusion_update(
            &fusion
        );

        ESP_LOGI(
    TAG,

    "B1:%ld Pa EMA:%ld W:%ld | "
    "B2:%ld Pa EMA:%ld W:%ld | "
    "B3:%ld Pa EMA:%ld W:%ld | "
    "FUSED:%ld Pa | ALT: %.2f m",

    (long)fusion.b1.pressure,
    (long)fusion.b1.ema,
    (long)fusion.b1.weight,

    (long)fusion.b2.pressure,
    (long)fusion.b2.ema,
    (long)fusion.b2.weight,

    (long)fusion.b3.pressure,
    (long)fusion.b3.ema,
    (long)fusion.b3.weight,

    (long)fusion.fused_pressure,

    fusion.altitude_relative
);

        vTaskDelay( pdMS_TO_TICKS(BARO_UPDATE_MS));
    }
}


/*
=========================
APP MAIN
=========================
*/
void app_main(void)
{
    ESP_ERROR_CHECK(
        i2c_master_init(
            I2C_NUM_0,
            GPIO_NUM_21,
            GPIO_NUM_22
        )
    );

    ESP_ERROR_CHECK(
        i2c_master_init(
            I2C_NUM_1,
            GPIO_NUM_19,
            GPIO_NUM_18
        )
    );

    baro_fusion_init(
        &fusion
    );

    fusion.b1.bmp.port= I2C_NUM_0;

    fusion.b2.bmp.port= I2C_NUM_1;

    fusion.b3.bmp.port= I2C_NUM_0;


  //Inicializar barometros

    if(
        !bmp280_init( &fusion.b1.bmp, 0x76)
    )
    {
        ESP_LOGE(TAG,"Erro BMP1");

        return;
    }
    fusion.b1.enabled=true;



    if(
        !bmp280_init( &fusion.b2.bmp, 0x76)
    )
    {
        ESP_LOGE(TAG,"Erro BMP2");

        return;
    }
    fusion.b2.enabled=true;



    /*if(
        !bmp280_init( &fusion.b3.bmp, 0x77)
    )
    {
        ESP_LOGE(TAG,"Erro BMP3");

        return;
    }
    fusion.b3.enabled=true;*/


    /*
    =========================
    ZERO CALIBRATION
    =========================
    */

    ESP_LOGI(
        TAG,
        "Calibrando zero..."
    );

    baro_fusion_calibrate_zero( &fusion, ZERO_SAMPLES, ZERO_DELAY_MS);

    ESP_LOGI(TAG,"Zero calibrado: %.2f m",fusion.altitude_offset);


    /*
    =========================
    CREATE TASK
    =========================
    */

    xTaskCreatePinnedToCore(
        task_baro,

        "task_baro",

        4096,

        NULL,

        1,

        NULL,

        1
    );
}