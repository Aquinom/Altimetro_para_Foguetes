#include "bmp280.h"
#include "i2c_helper.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <math.h>
#include <string.h>

#define BMP280_REG_ID      0xD0
#define BMP280_REG_CTRL    0xF4
#define BMP280_REG_CONFIG  0xF5
#define BMP280_REG_DATA    0xF7

static void read_calibration(bmp280_t *dev)
{
    uint8_t d[24];

    i2c_read_port(
        dev->port,
        dev->address,
        0x88,
        d,
        24);

    dev->calib.T1 = (d[1] << 8) | d[0];
    dev->calib.T2 = (d[3] << 8) | d[2];
    dev->calib.T3 = (d[5] << 8) | d[4];

    dev->calib.P1 = (d[7] << 8) | d[6];
    dev->calib.P2 = (d[9] << 8) | d[8];
    dev->calib.P3 = (d[11] << 8) | d[10];
    dev->calib.P4 = (d[13] << 8) | d[12];
    dev->calib.P5 = (d[15] << 8) | d[14];
    dev->calib.P6 = (d[17] << 8) | d[16];
    dev->calib.P7 = (d[19] << 8) | d[18];
    dev->calib.P8 = (d[21] << 8) | d[20];
    dev->calib.P9 = (d[23] << 8) | d[22];
}

static void read_raw(
    bmp280_t *dev,
    int32_t *adc_P,
    int32_t *adc_T)
{
    uint8_t raw[6];


    i2c_read_port(
        dev->port,
        dev->address,
        BMP280_REG_DATA,
        raw,
        6);

    *adc_P =
        ((raw[0] << 12) |
         (raw[1] << 4) |
         (raw[2] >> 4));

    *adc_T =
        ((raw[3] << 12) |
         (raw[4] << 4) |
         (raw[5] >> 4));
}

static void compensate_temp(
    bmp280_t *dev,
    int32_t adc_T)
{
    int32_t var1, var2;

    var1 =
        ((((adc_T >> 3) -
           ((int32_t)dev->calib.T1 << 1))) *
         ((int32_t)dev->calib.T2)) >>
        11;

    var2 =
        (((((adc_T >> 4) -
            ((int32_t)dev->calib.T1)) *
           ((adc_T >> 4) -
            ((int32_t)dev->calib.T1))) >>
          12) *
         ((int32_t)dev->calib.T3)) >>
        14;

    dev->t_fine = var1 + var2;
}

static float compensate_pressure(
    bmp280_t *dev,
    int32_t adc_P)
{
    int64_t var1, var2, p;

    var1 = ((int64_t)dev->t_fine) - 128000;

    var2 = var1 * var1 * (int64_t)dev->calib.P6;
    var2 = var2 + ((var1 * (int64_t)dev->calib.P5) << 17);
    var2 = var2 + (((int64_t)dev->calib.P4) << 35);

    var1 =
        ((var1 * var1 * (int64_t)dev->calib.P3) >> 8) +
        ((var1 * (int64_t)dev->calib.P2) << 12);

    var1 =
        (((((int64_t)1) << 47) + var1) *
         ((int64_t)dev->calib.P1)) >>
        33;

    if (var1 == 0)
        return 0;

    p = 1048576 - adc_P;

    p = (((p << 31) - var2) * 3125) / var1;

    var1 =
        (((int64_t)dev->calib.P9) *
         (p >> 13) *
         (p >> 13)) >>
        25;

    var2 =
        (((int64_t)dev->calib.P8) * p) >>
        19;

    p =
        ((p + var1 + var2) >> 8) +
        (((int64_t)dev->calib.P7) << 4);

    return p / 256.0f;
}

bool bmp280_init(
    bmp280_t *dev,
    i2c_port_t port,
    uint8_t addr)
{
    memset(dev, 0, sizeof(bmp280_t));

    dev->port = port;
    dev->address = addr;

    uint8_t id;

    if (i2c_read_port(
            dev->port,
            addr,
            BMP280_REG_ID,
            &id,
            1) != ESP_OK)
    {
        return false;
    }

    if (id != 0x58)
    {
        return false;
    }

    i2c_write_port(
        dev->port,
        addr,
        BMP280_REG_CTRL,
        0x27);

    i2c_write_port(
        dev->port,
        addr,
        BMP280_REG_CONFIG,
        0x10);

    read_calibration(dev);

    return true;
}

float bmp280_get_pressure(
    bmp280_t *dev)
{
    int32_t adc_P;
    int32_t adc_T;

    read_raw(
        dev,
        &adc_P,
        &adc_T);

    compensate_temp(
        dev,
        adc_T);

    return compensate_pressure(
        dev,
        adc_P);
}

void bmp280_calibrate_ground(
    bmp280_t *dev)
{
    const int samples = 100;

    float sum = 0.0f;

    for (int i = 0; i < samples; i++)
    {
        sum += bmp280_get_pressure(dev);

        vTaskDelay(
            pdMS_TO_TICKS(10));
    }

    dev->base_pressure =
        sum / samples;
}

float bmp280_get_altitude(
    bmp280_t *dev)
{
    float pressure =
        bmp280_get_pressure(dev);

    float altitude =
        44330.0f *
        (1.0f -
         powf(
             pressure /
                 dev->base_pressure,
             0.1903f));

    return altitude;
}