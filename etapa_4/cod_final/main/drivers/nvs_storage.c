#include "nvs_storage.h"

#include <stdio.h>

#include "nvs.h"
#include "nvs_flash.h"
#include "esp_check.h"

#include "esp_log.h"

#define STORAGE_NAMESPACE "flight"

static const char *TAG = "NVS";

static nvs_handle_t handle;

esp_err_t nvs_storage_init(void)
{
    esp_err_t err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
        err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }

    ESP_RETURN_ON_ERROR(err, TAG, "NVS init");

    err = nvs_open(STORAGE_NAMESPACE,
                   NVS_READWRITE,
                   &handle);

    return err;
}

esp_err_t nvs_save_max_altitude(float altitude)
{
    uint32_t counter = 0;

    nvs_get_u32(handle,
                "counter",
                &counter);

    uint8_t slot = counter % NVS_MAX_FLIGHTS;

    char key[8];

    sprintf(key, "A%d", slot);

    ESP_ERROR_CHECK(
        nvs_set_blob(handle,
                     key,
                     &altitude,
                     sizeof(float)));

    counter++;

    ESP_ERROR_CHECK(
        nvs_set_u32(handle,
                    "counter",
                    counter));

    ESP_ERROR_CHECK(
        nvs_commit(handle));

    return ESP_OK;
}

bool nvs_get_altitude(uint8_t slot,
                      float *altitude)
{
    char key[8];

    sprintf(key, "A%d", slot);

    size_t size = sizeof(float);

    return nvs_get_blob(handle,
                        key,
                        altitude,
                        &size) == ESP_OK;
}

uint32_t nvs_get_counter(void)
{
    uint32_t counter = 0;

    nvs_get_u32(handle,
                "counter",
                &counter);

    return counter;
}

void nvs_print_history(void)
{
    uint32_t counter = nvs_get_counter();

    ESP_LOGI("NVS", "===============================");
    ESP_LOGI("NVS", "Projeto Integrador III");
    ESP_LOGI("NVS", "Caio Meira");
    ESP_LOGI("NVS", "Marcelo Zampieri");
    ESP_LOGI("NVS", "Rafael Aquino");
    ESP_LOGI("NVS", "========== HISTORICO ==========");

    if(counter == 0)
    {
        ESP_LOGI("NVS", "Nenhum voo encontrado.");
        return;
    }

    uint32_t total =
        counter < NVS_MAX_FLIGHTS ?
        counter :
        NVS_MAX_FLIGHTS;

    uint32_t first =
        (counter >= NVS_MAX_FLIGHTS) ?
        counter % NVS_MAX_FLIGHTS :
        0;

    for(uint32_t i=0;i<total;i++)
    {
        uint32_t slot =
            (first+i)%NVS_MAX_FLIGHTS;

        float alt;

        if(nvs_get_altitude(slot,&alt))
        {
            ESP_LOGI("NVS",
                     "Voo %lu -> %.2f m",
                     counter-total+i+1,
                     alt);
        }
    }

    ESP_LOGI("NVS","===============================");
}