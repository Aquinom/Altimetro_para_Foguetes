Telemetria para Foguetes com ESP32
==================================

Sistema de telemetria embarcada utilizando ESP32, ESP-IDF e FreeRTOS para leitura e processamento de dados em tempo real durante o voo do foguete.

Conexões de hardware
--------------------

ESP32 DevKit:

- GPIO 21 → SDA (BMP280 e MPU6050)
- GPIO 22 → SCL (BMP280 e MPU6050)
- 3V3 → VCC (BMP280 e MPU6050)
- 3v3 → CSB (BMP280)
- GND → GND (BMP280 e MPU6050)
- GND → SDO (BMP280)
- GND → AD0 (MPU6050)

Sensores utilizados:

- BMP280
- MPU6050

Estrutura do Projeto
--------------------

::

    main/
     ├── main.c
     ├── flight_controller.c
     ├── flight_controller.h
     └── drivers/
          ├── bmp280/
          │   ├── bmp280.h
          │   └── bmp280.c
          ├── mpu6050/
          │   ├── mpu6050.h
          │   └── mpu6050.c
          └── i2c/
              ├── i2c_helper.h
              └── i2c_helper.c


Descrição
---------

O sistema utiliza comunicação I2C para realizar a leitura contínua dos sensores embarcados, obtendo dados de:

- Pressão atmosférica;
- Altitude;
- Temperatura;
- Aceleração nos eixos;
- Velocidade vertical estimada.

A aplicação foi estruturada utilizando FreeRTOS, separando a leitura dos sensores e a telemetria em tasks independentes.

A task principal de sensores realiza aquisição e processamento dos dados:

::

    xTaskCreatePinnedToCore(
        sensor_task,
        "sensor_task",
        4096,
        NULL,
        5,
        NULL,
        1);

Enquanto isso, outra task é responsável apenas pela telemetria serial:

::

    xTaskCreatePinnedToCore(
        telemetry_task,
        "telemetry_task",
        4096,
        NULL,
        1,
        NULL,
        0);

Os dados obtidos passam por filtros de média móvel para reduzir ruídos e oscilações causadas por vibrações durante o voo.

O filtro utilizado possui 15 amostras:

::

    #define MOVING_AVG_SIZE 15

A atualização da média móvel é feita continuamente durante a execução:

::

    altitude = ma_update(&alt_filter, altitude_raw);

A velocidade vertical é estimada combinando dados do barômetro e acelerômetro:

::

    velocity += accel * dt * 0.25f;

    velocity =
        (velocity * 0.35f) +
        (vel_baro * 0.65f);

O sistema também implementa detecção de apogeu através da análise da altitude, velocidade vertical e aceleração do foguete.

A detecção ocorre quando:

- O foguete ultrapassa uma altitude mínima;
- A velocidade vertical torna-se negativa;
- A aceleração indica início da descida.

::

    if (
        altitude_valid &&
        descending &&
        accel_negative)
    {
        apogee_detected = true;

        ESP_LOGW(TAG, "APOGEU DETECTADO");
    }

Mutex são utilizados para proteger os dados compartilhados entre as tasks:

::

    xSemaphoreTake(telemetry_mutex, portMAX_DELAY);

    telemetry.altitude = altitude;

    xSemaphoreGive(telemetry_mutex);

Os dados processados são enviados continuamente ao monitor serial:

::

    ESP_LOGI(
        TAG,
        "ALT: %.2f m | VEL: %.2f m/s | ACC: %.3f g",
        local.altitude,
        local.velocity,
        local.acceleration);



Referências
------------

- `Documentação ESP-IDF <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_
- `Documentação FreeRTOS <https://www.freertos.org/Documentation/RTOS_book.html>`_
- `Documentação I2C ESP32 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_
- `Exemplos ESP-IDF Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_
- `Datasheet BMP280 <https://www.bosch-sensortec.com/products/environmental-sensors/pressure-sensors/bmp280/>`_
- `Datasheet MPU6050 <https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/>`_
