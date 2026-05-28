idf_component_register(
    SRCS
        "main.c"
        "flight_controller.c"
        "../drivers/i2c/i2c_helper.c"
        "../drivers/bmp280/bmp280.c"
        "../drivers/mpu6050/mpu6050.c"

    INCLUDE_DIRS
        "."
        "../drivers/i2c"
        "../drivers/bmp280"
        "../drivers/mpu6050"
)