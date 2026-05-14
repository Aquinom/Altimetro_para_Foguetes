Teste do barômetro BMP280 com ESP32
===================================

Teste do barômetro BMP280 utilizando ESP32, em C com ESP-IDF.

Conexões de hardware
--------------------

ESP32 DevKit:

- GPIO 21 → SDA
- GPIO 22 → SCL
- VCC → VCC
- GND → GND

BMP280:

- VCC → VCC
- GND → GND
- SDA → GPIO 21
- SCL → GPIO 22




Estrutura do Projeto
--------------------

::

    main/
     ├── main.c
     └── drivers/
          ├── i2c/
          │    ├── i2c_helper.h
          │    └── i2c_helper.c
          └── bmp280/
               ├── bmp280.h
               └── bmp280.c


Main.c
------

.. code-block:: c

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


bmp280.h
--------

.. code-block:: c

    #pragma once

    #include <stdbool.h>
    #include <stdint.h>

    typedef struct
    {
        uint16_t T1;
        int16_t T2;
        int16_t T3;

        uint16_t P1;
        int16_t P2;
        int16_t P3;
        int16_t P4;
        int16_t P5;
        int16_t P6;
        int16_t P7;
        int16_t P8;
        int16_t P9;

    } bmp280_calib_t;


    typedef struct
    {
        uint8_t address;

        bmp280_calib_t calib;

        int32_t t_fine;

        float altitude_offset;
        float last_pressure;

    } bmp280_t;


    bool bmp280_init(
        bmp280_t *dev,
        uint8_t addr
    );

    float bmp280_get_pressure(
        bmp280_t *dev
    );

    float bmp280_get_altitude(
        bmp280_t *dev
    );


bmp280.c
--------

.. code-block:: c

    #include "bmp280.h"
    #include "../i2c/i2c_helper.h"
    #include <math.h>

    #define BMP280_REG_ID      0xD0
    #define BMP280_REG_CTRL    0xF4
    #define BMP280_REG_CONFIG  0xF5
    #define BMP280_REG_DATA    0xF7

    #define SEA_LEVEL_PRESSURE 102000.0
    #define FILTER_SIZE 10

    static float filter_buf[FILTER_SIZE];
    static int filter_idx = 0;
    static int filter_count = 0;


    static float moving_average(float x)
    {
        filter_buf[filter_idx] = x;

        filter_idx =
            (filter_idx + 1) % FILTER_SIZE;

        if (filter_count < FILTER_SIZE)
            filter_count++;

        float sum = 0;

        for (int i = 0;
             i < filter_count;
             i++)
        {
            sum += filter_buf[i];
        }

        return sum / filter_count;
    }


    static float calc_alt(float p)
    {
        return 44330.0 *
               (1.0 - pow(
                    p / SEA_LEVEL_PRESSURE,
                    0.1903
               ));
    }


    bool bmp280_init(
        bmp280_t *dev,
        uint8_t addr
    )
    {
        dev->address = addr;

        // leitura ID
        // leitura calibração
        // configuração oversampling
        // configuração filtro IIR
        // cálculo do offset inicial

        return true;
    }


    float bmp280_get_pressure(
        bmp280_t *dev
    )
    {
        // leitura e compensação
        return dev->last_pressure;
    }


    float bmp280_get_altitude(
        bmp280_t *dev
    )
    {
        return moving_average(
            calc_alt(dev->last_pressure)
            - dev->altitude_offset
        );
    }


i2c_helper.h
------------

.. code-block:: c

    #pragma once

    #include "esp_err.h"
    #include <stdint.h>
    #include <stddef.h>

    esp_err_t i2c_master_init(void);

    esp_err_t i2c_write(
        uint8_t addr,
        uint8_t reg,
        uint8_t data
    );

    esp_err_t i2c_read(
        uint8_t addr,
        uint8_t reg,
        uint8_t *data,
        size_t len
    );


i2c_helper.c
------------

.. code-block:: c

    #include "i2c_helper.h"
    #include "driver/i2c.h"

    #define SDA_PIN 21
    #define SCL_PIN 22

    #define I2C_PORT I2C_NUM_0
    #define FREQ 100000


    esp_err_t i2c_master_init(void)
    {
        i2c_config_t conf =
        {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = SDA_PIN,
            .scl_io_num = SCL_PIN,
            .sda_pullup_en =
                GPIO_PULLUP_ENABLE,
            .scl_pullup_en =
                GPIO_PULLUP_ENABLE,
            .master.clk_speed = FREQ
        };

        i2c_param_config(
            I2C_PORT,
            &conf
        );

        return i2c_driver_install(
            I2C_PORT,
            conf.mode,
            0,
            0,
            0
        );
    }
