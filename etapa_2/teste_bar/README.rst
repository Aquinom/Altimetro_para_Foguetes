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

O código implementa a leitura de pressão atmosférica, temperatura e altitude utilizando o sensor BMP280 conectado a um ESP32 por meio da comunicação I2C.

A inicialização da interface I2C é realizada no módulo ``i2c_helper.c``, responsável por configurar o barramento I2C nos pinos GPIO 21 (SDA) e GPIO 22 (SCL), permitindo a comunicação entre o ESP32 e o sensor BMP280.

Após a configuração do barramento, a função de inicialização presente em ``bmp280.c`` realiza a verificação da comunicação com o sensor e configura seus registradores internos para operação contínua.

A leitura dos dados é feita por meio do acesso aos registradores internos do BMP280, obtendo inicialmente os valores brutos de pressão e temperatura fornecidos pelo sensor.

Esses valores passam pelo processo de compensação definido pelo fabricante, utilizando os coeficientes de calibração armazenados na memória interna do próprio módulo, permitindo calcular medições mais precisas.

A conversão para altitude é realizada a partir da equação barométrica:

::

    altitude = 44330 * (1 - pow(pressao / pressao_referencia, 0.1903))

onde:

- ``pressao`` representa a pressão atmosférica medida pelo sensor;
- ``pressao_referencia`` representa a pressão ao nível do mar, utilizada como referência.

No arquivo ``main.c``, uma tarefa principal executa leituras periódicas do BMP280 e imprime no monitor serial os valores de temperatura, pressão e altitude em tempo real, permitindo validar o funcionamento do sensor e acompanhar variações ambientais durante os testes.


Referências
---------
- `Tutorial I2C ESP32 <https://microcontrollerslab.com/esp32-i2c-communication-tutorial-arduino-ide>`_
- `Documentação Espressif <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_
- `Documentação I2C utilizado para programar o MPU6050 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_
- `Exemplos de programação ESP32 Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_
- `ESP32 as I2C Master <https://www.youtube.com/watch?v=Snp6iTu1R7E>`_

