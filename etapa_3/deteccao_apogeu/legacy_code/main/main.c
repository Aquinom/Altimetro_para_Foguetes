#include "drivers/i2c/i2c_helper.h"
#include "drivers/mpu6050/mpu6050.h"

void app_main(void)
{
    ESP_ERROR_CHECK(
        i2c_master_init()
    );

    mpu6050_init();

    while (1)
    {
        mpu6050_update();
    }
}