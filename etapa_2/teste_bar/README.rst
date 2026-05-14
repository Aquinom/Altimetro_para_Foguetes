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
    #include "i2c_helper.h"
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

        for (int i = 0; i < filter_count; i++)
            sum += filter_buf[i];

        return sum / filter_count;
    }


    static void read_calibration(bmp280_t *dev)
    {
        uint8_t d[24];

        i2c_read(dev->address, 0x88, d, 24);

        dev->calib.T1=(d[1]<<8)|d[0];
        dev->calib.T2=(d[3]<<8)|d[2];
        dev->calib.T3=(d[5]<<8)|d[4];

        dev->calib.P1=(d[7]<<8)|d[6];
        dev->calib.P2=(d[9]<<8)|d[8];
        dev->calib.P3=(d[11]<<8)|d[10];
        dev->calib.P4=(d[13]<<8)|d[12];
        dev->calib.P5=(d[15]<<8)|d[14];
        dev->calib.P6=(d[17]<<8)|d[16];
        dev->calib.P7=(d[19]<<8)|d[18];
        dev->calib.P8=(d[21]<<8)|d[20];
        dev->calib.P9=(d[23]<<8)|d[22];
    }


    static void compensate_temp(
        bmp280_t *dev,
        int32_t adc_T
    )
    {
        int32_t v1, v2;

        v1 =
            ((((adc_T >> 3) -
            ((int32_t)dev->calib.T1 << 1))) *
            ((int32_t)dev->calib.T2)) >> 11;

        v2 =
            (((((adc_T >> 4) -
            ((int32_t)dev->calib.T1)) *
            ((adc_T >> 4) -
            ((int32_t)dev->calib.T1))) >> 12) *
            ((int32_t)dev->calib.T3)) >> 14;

        dev->t_fine = v1 + v2;
    }


    static float compensate_pressure(
        bmp280_t *dev,
        int32_t adc_P
    )
    {
        int64_t v1, v2, p;

        v1 = ((int64_t)dev->t_fine) - 128000;

        v2 = v1 * v1 * (int64_t)dev->calib.P6;
        v2 += ((v1 * (int64_t)dev->calib.P5) << 17);
        v2 += (((int64_t)dev->calib.P4) << 35);

        v1 =
            ((v1 * v1 * (int64_t)dev->calib.P3) >> 8)
            + ((v1 * (int64_t)dev->calib.P2) << 12);

        v1 =
            ((((int64_t)1 << 47) + v1) *
            ((int64_t)dev->calib.P1)) >> 33;

        if (v1 == 0)
            return 0;

        p = 1048576 - adc_P;
        p = (((p << 31) - v2) * 3125) / v1;

        v1 =
            (((int64_t)dev->calib.P9) *
            (p >> 13) * (p >> 13)) >> 25;

        v2 =
            (((int64_t)dev->calib.P8) * p) >> 19;

        p =
            ((p + v1 + v2) >> 8) +
            (((int64_t)dev->calib.P7) << 4);

        return p / 256.0;
    }


    static void read_raw(
        bmp280_t *dev,
        int32_t *adc_P,
        int32_t *adc_T
    )
    {
        uint8_t raw[6];

        i2c_read(
            dev->address,
            BMP280_REG_DATA,
            raw,
            6
        );

        *adc_P =
            ((raw[0] << 12) |
             (raw[1] << 4)  |
             (raw[2] >> 4));

        *adc_T =
            ((raw[3] << 12) |
             (raw[4] << 4)  |
             (raw[5] >> 4));
    }


    static float calc_alt(float p)
    {
        return 44330.0 *
               (1.0 -
                pow(
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

        uint8_t id;

        i2c_read(addr, BMP280_REG_ID, &id, 1);

        if (id != 0x58)
            return false;

        read_calibration(dev);

        i2c_write(addr, BMP280_REG_CTRL, 0x57);
        i2c_write(addr, BMP280_REG_CONFIG, 0x14);

        int32_t p, t;

        read_raw(dev, &p, &t);

        compensate_temp(dev, t);

        dev->altitude_offset =
            calc_alt(
                compensate_pressure(dev, p)
            );

        return true;
    }


    float bmp280_get_pressure(
        bmp280_t *dev
    )
    {
        int32_t p, t;

        read_raw(dev, &p, &t);

        compensate_temp(dev, t);

        dev->last_pressure =
            compensate_pressure(dev, p);

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


    esp_err_t i2c_write(
        uint8_t addr,
        uint8_t reg,
        uint8_t data
    )
    {
        uint8_t buf[2] =
        {
            reg,
            data
        };

        return i2c_master_write_to_device(
            I2C_PORT,
            addr,
            buf,
            2,
            1000 / portTICK_PERIOD_MS
        );
    }


    esp_err_t i2c_read(
        uint8_t addr,
        uint8_t reg,
        uint8_t *data,
        size_t len
    )
    {
        return i2c_master_write_read_device(
            I2C_PORT,
            addr,
            &reg,
            1,
            data,
            len,
            1000 / portTICK_PERIOD_MS
        );
    }
