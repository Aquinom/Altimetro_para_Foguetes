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


Descrição
---------


Referências
---------
- `Tutorial I2C ESP32 <https://microcontrollerslab.com/esp32-i2c-communication-tutorial-arduino-ide>`_
- `Documentação Espressif <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_
- `Documentação I2C utilizado para programar o MPU6050 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_
- `Exemplos de programação ESP32 Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_
- `ESP32 as I2C Master <https://www.youtube.com/watch?v=Snp6iTu1R7E>`_

