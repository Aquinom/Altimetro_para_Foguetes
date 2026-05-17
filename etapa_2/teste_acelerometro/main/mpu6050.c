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