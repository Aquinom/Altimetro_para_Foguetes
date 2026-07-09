//====================================================
// INCLUDES INTERNOS
//====================================================
#include "statemachine/statemachine.h"
#include "tasks/init_task.h"

#include "drivers/servo.h"
#include "drivers/buzzer.h"
#include "tasks/acquire_task.h"
#include "tasks/flight_task.h"
#include "flight_data.h"

//====================================================
// INCLUDES EXTERNOS
//====================================================
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdbool.h>

#include "esp_log.h"
#include "esp_err.h"

//====================================================
// PRIVATE VARIABLES
//====================================================

static state_t current_state = PRELAUNCH;

static const char *TAG = "STATE";

static TickType_t last_beep = 0;

static TickType_t last_flight_log = 0;

//====================================================
// PRIVATE PROTOTYPES
//====================================================

static void statemachine_task(void *arg);

static void change_state(state_t new_state);

static void state_prelaunch(void);
static void state_launch(void);
static void state_flying(void);
static void state_peak(void);
static void state_findme(void);

static bool active_peak_detected(void);
static bool restart_requested(void);


//====================================================
// PUBLIC FUNCTIONS
//====================================================

void state_machine_task_start(void)
{
    xTaskCreatePinnedToCore(
        statemachine_task,
        "StateMachine",
        4096,
        NULL,
        2,
        NULL,
        1);
}


//====================================================
// PRIVATE FUNCTIONS
//====================================================

static void statemachine_task(void *arg)
{
    (void)arg;

    current_state = PRELAUNCH;

    for (;;)
    {
        switch (current_state)
        {
            case PRELAUNCH:

                state_prelaunch();

                change_state(LAUNCH);

                break;

            case LAUNCH:

                state_launch();

                change_state(FLYING);

                break;

            case FLYING:

                state_flying();

                if (active_peak_detected())
                {
                    change_state(PEAK);
                }

                //change_state(PEAK); //provisório

                break;

            case PEAK:

                state_peak();

                change_state(FINDME);

                break;

            case FINDME:

                state_findme();

                if (restart_requested())
                {
                    change_state(PRELAUNCH);
                }

                break;

            default:

                change_state(PRELAUNCH);

                break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}


static void change_state(state_t new_state)
{
    current_state = new_state;
}


//====================================================
// STATES
//====================================================

static void state_prelaunch(void)
{
    /*
     * Inicialização do sistema.
     *
     * Exemplo:
     * - Inicializar sensores
     * - Calibrar pressão
     * - Criar outras tasks
     * - Esperar confirmação de lançamento
     */
    ESP_LOGI(TAG, "INICIO PRELAUNCH");

    init_task();

    ESP_LOGI(TAG, "FIM PRELAUNCH");
}


static void state_launch(void)
{
    /*
     * Executado logo após detectar o lançamento.
     *
     * Exemplo:
     * - Registrar timestamp
     * - Habilitar gravação
     * - Iniciar telemetria
     */

    ESP_LOGI(TAG, "INICIO LAUNCH");

    ESP_LOGI("INIT", "Starting tasks...");

    acquire_task_start();
    flight_task_start();

    ESP_LOGI(TAG, "FIM LAUNCH");
}


static void state_flying(void)
{
    /*
     * Estado principal do voo.
     *
     * Exemplo:
     * - Acompanhar altitude
     * - Atualizar velocidade
     * - Verificar apogeu
     */
    //ESP_LOGI(TAG, "INICIO FLYING");

    flight_data_t data;

    xSemaphoreTake(
        g_flight_data_mutex,
        portMAX_DELAY);

    data = g_flight_data;

    xSemaphoreGive(
        g_flight_data_mutex);


    if(xTaskGetTickCount() - last_flight_log >
        pdMS_TO_TICKS(1000))
    {
        ESP_LOGI(TAG,
            "P: %.2f | Alt: %.2f | FAlt: %.2f | Vel: %.2f | Acc: %.2f",
            data.pressure,
            data.altitude,
            data.filtered_altitude,
            data.velocity,
            data.acceleration);

        last_flight_log = xTaskGetTickCount();
    }
    
    //ESP_LOGI(TAG, "FIM FLYING");

    //vTaskDelete(NULL);
}


static void state_peak(void)
{
    ESP_LOGI(TAG, "INICIO PEAK");


    float altitude =
        flight_peak_altitude();


    ESP_LOGI(TAG,
             "Altitude maxima: %.2f",
             altitude);

    acquire_task_delete();

    flight_task_delete();

    servo_open();

    ESP_LOGI(TAG, "FIM PEAK");
}


static void state_findme(void)
{
    if(
        xTaskGetTickCount() - last_beep >
        pdMS_TO_TICKS(5000))
    {
        buzzer_beep_ms(200);

        last_beep =
            xTaskGetTickCount();
    }
}


//====================================================
// TRANSITIONS
//====================================================

static bool active_peak_detected(void)
{
    /*
     * TODO:
     * Substituir pela lógica real de detecção do apogeu.
     */

    return flight_peak_detected();
}


static bool restart_requested(void)
{
    /*
     * TODO:
     * Substituir pela lógica real de reinicialização.
     */

    return false;
}
