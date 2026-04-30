Teste do servo motor SG90 com ESP32
==================================

Controle de servo realizado utilizando ESP32, em C com ESP-IDF.

Estrutura do Projeto
--------------------

::

    main/
     ├── main.c
     ├── servo.h
     ├── servo.c
     ├── teste.h
     ├── teste.c

Main.c
------

.. code-block:: c

    #include "teste.h"

    void app_main(void)
    {
        teste_start();
    }

Servo.h
-------

.. code-block:: c

    #pragma once

    #include <stdint.h>

    void servo_init(int gpio);
    void servo_set_angle(int angle);

Servo.c
-------

.. code-block:: c

    #include "servo.h"
    #include "driver/ledc.h"
    #include "esp_err.h"

    #define LEDC_TIMER        LEDC_TIMER_0
    #define LEDC_MODE         LEDC_LOW_SPEED_MODE
    #define LEDC_CHANNEL      LEDC_CHANNEL_0
    #define LEDC_DUTY_RES     LEDC_TIMER_16_BIT
    #define LEDC_FREQUENCY    50

    #define SERVO_MIN_PULSEWIDTH_US 500
    #define SERVO_MAX_PULSEWIDTH_US 2500
    #define SERVO_MAX_DEGREE        180
    #define SERVO_PERIOD_US         20000

    static int s_gpio = 18;

    static uint32_t angle_to_duty(int angle)
    {
        if (angle < 0) angle = 0;
        if (angle > SERVO_MAX_DEGREE) angle = SERVO_MAX_DEGREE;

        uint32_t pulse_width = SERVO_MIN_PULSEWIDTH_US +
            (angle * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US)) / SERVO_MAX_DEGREE;

        return (pulse_width * (1 << LEDC_DUTY_RES)) / SERVO_PERIOD_US;
    }

    void servo_init(int gpio)
    {
        s_gpio = gpio;

        ledc_timer_config_t timer = {
            .speed_mode       = LEDC_MODE,
            .duty_resolution  = LEDC_DUTY_RES,
            .timer_num        = LEDC_TIMER,
            .freq_hz          = LEDC_FREQUENCY,
            .clk_cfg          = LEDC_AUTO_CLK
        };

        if (ledc_timer_config(&timer) != ESP_OK) {
            // erro na configuração do timer
        }

        ledc_channel_config_t channel = {
            .gpio_num   = s_gpio,
            .speed_mode = LEDC_MODE,
            .channel    = LEDC_CHANNEL,
            .intr_type  = LEDC_INTR_DISABLE,
            .timer_sel  = LEDC_TIMER,
            .duty       = 0,
            .hpoint     = 0
        };

        if (ledc_channel_config(&channel) != ESP_OK) {
            // erro na configuração do canal
        }
    }

    void servo_set_angle(int angle)
    {
        uint32_t duty = angle_to_duty(angle);

        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    }

Teste.c
-------

.. code-block:: c

    #include "teste.h"
    #include "servo.h"
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"

    #define SERVO_GPIO 18

    static void servo_test_task(void *arg)
    {
        while (1)
        {
            servo_set_angle(0);
            vTaskDelay(pdMS_TO_TICKS(2000));

            servo_set_angle(90);
            vTaskDelay(pdMS_TO_TICKS(2000));

            servo_set_angle(180);
            vTaskDelay(pdMS_TO_TICKS(2000));
        }
    }

    void teste_start(void)
    {
        servo_init(SERVO_GPIO);

        xTaskCreate(
            servo_test_task,
            "servo_test_task",
            2048,
            NULL,
            5,
            NULL
        );
    }

Teste.h
-------

.. code-block:: c

    #pragma once

    void teste_start(void);

Conexões hardware
--------

ESP32:

- GPIO 18 → Sinal do servo (SG90)
- GND → GND comum

Servo SG90:

- Vermelho (VCC) → Saída 5V do regulador
- Marrom (GND) → GND comum
- Laranja (Sinal) → GPIO 18

Bateria 2S (7.4V):

- Positivo → Entrada do regulador
- Negativo → GND comum

Regulador:

- Entrada → Bateria 2S
- Saída 5V → Servo
- GND → GND comum (ESP32 + servo + bateria)

