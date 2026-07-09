idf_component_register(

    SRCS

        "main.c"

        "tasks/init_task.c"
        "tasks/acquire_task.c"
        "tasks/flight_task.c"

        "drivers/sensor_context.c"
        "drivers/bmp280.c"
        "drivers/buzzer.c"
        "drivers/i2c_helper.c"
        "drivers/mpu6050.c"
        "drivers/servo.c"
        "drivers/nvs_storage.c"

        "filter/kalman_altitude.c"

        "statemachine/statemachine.c"

    INCLUDE_DIRS

        "."
        "drivers"
        "filter"
        "tasks"
        "statemachine"

    REQUIRES

        nvs_flash
        driver
        freertos
        esp_timer
        esp_driver_gpio
        esp_driver_i2c
        esp_driver_ledc
)