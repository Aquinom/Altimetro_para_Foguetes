
//====================================================
// INCLUDES INTERNOS
//====================================================
#include "statemachine/statemachine.h"

#include "drivers/nvs_storage.h"

//====================================================
// INCLUDES EXTERNOS
//====================================================
#include <stdio.h>
#include "esp_log.h"
#include "esp_err.h"




//====================================================
// APP MAIN
//====================================================
void app_main(void)
{
    ESP_ERROR_CHECK(nvs_storage_init());

    nvs_print_history();
    /*
     * Inicializa a máquina de estados.
     * Toda a lógica do voo é controlada por ela.
     */
    state_machine_task_start();
}

