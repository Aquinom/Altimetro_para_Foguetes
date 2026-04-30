Teste do acelerômetro MPU6050 com ESP32
======================================

Teste do acelerômetro MPU6050 utilizando ESP32, em C com ESP-IDF.

Conexões de hardware
--------

ESP32 DevKit:

- GPIO 21 → SDA
- GPIO 22 → SCL
- 3.3V → VCC do MPU6050
- GND → GND comum

MPU6050:

- VCC → 3.3V
- GND → GND
- SDA → GPIO 21
- SCL → GPIO 22


Estrutura do Projeto
--------------------

::

    main/
     ├── main.c
     ├── mpu6050.h
     ├── mpu6050.c
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

mpu6050.h
---------

.. code-block:: c

    #pragma once

    #include <stdint.h>

    typedef struct
    {
        float ax;
        float ay;
        float az;
    } mpu6050_data_t;

    void mpu6050_init(void);
    void mpu6050_read_accel(mpu6050_data_t *data);

mpu6050.c
---------

.. code-block:: c

    #include "mpu6050.h"
    #include "driver/i2c.h"
    #include "esp_err.h"

    #define I2C_MASTER_SCL_IO           22
    #define I2C_MASTER_SDA_IO           21
    #define I2C_MASTER_NUM              I2C_NUM_0
    #define I2C_MASTER_FREQ_HZ          100000

    #define MPU6050_ADDR                0x68
    #define MPU6050_PWR_MGMT_1          0x6B
    #define MPU6050_ACCEL_XOUT_H        0x3B

    static void i2c_init(void)
    {
        i2c_config_t conf = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = I2C_MASTER_SDA_IO,
            .scl_io_num = I2C_MASTER_SCL_IO,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = I2C_MASTER_FREQ_HZ,
        };

        i2c_param_config(I2C_MASTER_NUM, &conf);
        i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    }

    void mpu6050_init(void)
    {
        i2c_init();

        uint8_t data[2] = {MPU6050_PWR_MGMT_1, 0x00};

        i2c_master_write_to_device(
            I2C_MASTER_NUM,
            MPU6050_ADDR,
            data,
            2,
            1000 / portTICK_PERIOD_MS
        );
    }

    void mpu6050_read_accel(mpu6050_data_t *out)
    {
        uint8_t reg = MPU6050_ACCEL_XOUT_H;
        uint8_t data[6];

        i2c_master_write_read_device(
            I2C_MASTER_NUM,
            MPU6050_ADDR,
            &reg,
            1,
            data,
            6,
            1000 / portTICK_PERIOD_MS
        );

        int16_t ax = (data[0] << 8) | data[1];
        int16_t ay = (data[2] << 8) | data[3];
        int16_t az = (data[4] << 8) | data[5];

        out->ax = ax / 16384.0;
        out->ay = ay / 16384.0;
        out->az = az / 16384.0;
    }

Teste.c
-------

.. code-block:: c

    #include "teste.h"
    #include "mpu6050.h"
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include <stdio.h>
    #include <math.h>

    static int deploy_done = 0;

    // Placeholder para integração futura com barômetro
    static float last_pressure = 0.0;

    static float calc_accel_magnitude(mpu6050_data_t *d)
    {
        return sqrtf(d->ax * d->ax +
                     d->ay * d->ay +
                     d->az * d->az);
    }

    static void mpu_task(void *arg)
    {
        mpu6050_data_t data;

        while (1)
        {
            mpu6050_read_accel(&data);

            float accel_mag = calc_accel_magnitude(&data);

            printf("AX: %.2f AY: %.2f AZ: %.2f | |A|: %.2f\n",
                   data.ax, data.ay, data.az, accel_mag);

            // ===============================
            // Lógica futura de ativação do servo SG90 com barômetro e acelerômetro
            // ===============================

            if (!deploy_done)
            {
                // condição 1: baixa aceleração (~queda livre)
                if (accel_mag < 0.3f)
                {
                    // Substituir por leitura real do barômetro
                    float pressure = last_pressure;

                    // Se a pressão estiver aumentando (descendo)
                    if (pressure > last_pressure)
                    {
                        printf(">>> CONDIÇÃO DE DEPLOY DETECTADA <<<\n");

                        // Chamar servo_set_angle(180) e ativar o paraquedas;
                        // servo_set_angle(180);

                        deploy_done = 1;
                    }

                    last_pressure = pressure;
                }
            }

            vTaskDelay(pdMS_TO_TICKS(500));
        }
    }

    void teste_start(void)
    {
        mpu6050_init();

        xTaskCreate(
            mpu_task,
            "mpu_task",
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
