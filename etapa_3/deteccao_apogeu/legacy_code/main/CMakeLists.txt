idf_component_register(
    SRCS
        "main.c"
        "drivers/i2c/i2c_helper.c"
        "drivers/mpu6050/mpu6050.c"

    INCLUDE_DIRS
        "."
        "drivers/i2c"
        "drivers/mpu6050"
)