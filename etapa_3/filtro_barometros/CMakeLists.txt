idf_component_register(
    SRCS
        "main.c"
        "drivers/i2c/i2c_helper.c"
        "drivers/bmp280/bmp280.c"
        "drivers/sensor_fusion/baro_fusion.c"

    INCLUDE_DIRS
        "."
        "drivers/i2c"
        "drivers/bmp280"
        "drivers/sensor_fusion"
)