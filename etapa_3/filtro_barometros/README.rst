Implementação de Três Barômetros com Filtro
==================================

Sistema que integra 3 barômetros diferentes e aplica um filtro para melhorar estabilidade das leituras.

Conexões de hardware
--------------------

ESP32 DevKit:

- 3V3 → VCC (BMP280_0 , BMP280_1 , BMP280_2)
- GND → GND ((BMP280_0 , BMP280_1 , BMP280_2)
- GPIO 21 → SDA (BMP280_0 e BMP280_1)
- GPIO 22 → SCL (BMP280_0 e BMP280_1)
- GPIO 19 → SDA (BMP280_2)
- GPIO 18 → SCL (BMP280_2)
- DESCONECTADO → CSE (BMP280_0 , BMP280_1 , BMP280_2)
- DESCONECTADO → AD0 (BMP280_0 e BMP280_1)
- GND → AD0 (BMP280_2)

Sensores utilizados:

- 3x BMP280

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
          ├── sensor_fusion/
          │   ├── baro_fusion.h
          │   └── baro_fusion.c
          └── i2c/
              ├── i2c_helper.h
              └── i2c_helper.c


Descrição
---------

O sistema utiliza comunicação I2C para realizar a leitura contínua dos sensores embarcados, obtendo dados de:

- Pressão atmosférica;
- Altitude;
- Temperatura;

A aplicação foi estruturada utilizando FreeRTOS, desse modo a task atualiza constantemente os dados dos sensores e mostra no monitor serial.

A task principal de sensores realiza aquisição e processamento dos dados:

::

    xTaskCreatePinnedToCore(
        task_baro,
        "task_baro",
        4096,
        NULL,
        1,
        NULL,
        1);





Referências
------------

- `Documentação ESP-IDF <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_
- `Documentação FreeRTOS <https://www.freertos.org/Documentation/RTOS_book.html>`_
- `Documentação I2C ESP32 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_
- `Exemplos ESP-IDF Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_
- `Datasheet BMP280 <https://www.bosch-sensortec.com/products/environmental-sensors/pressure-sensors/bmp280/>`_
- `Datasheet MPU6050 <https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/>`_
