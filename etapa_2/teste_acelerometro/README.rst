Teste do acelerômetro MPU6050 com ESP32
======================================

Teste do acelerômetro MPU6050 utilizando ESP32, em C com ESP-IDF.

Conexões de hardware
--------

ESP32 DevKit:

- GPIO 21 → SDA
- GPIO 22 → SCL
- 3.3V → VCC do MPU6050
- GND → GND comum

MPU6050:

- VCC → 3.3V
- GND → GND
- SDA → GPIO 21
- SCL → GPIO 22


Estrutura do Projeto
--------------------

::

    main/
     ├── main.c
     ├── mpu6050.h
     ├── mpu6050.c
     ├── teste.h
     ├── teste.c

Descrição
---------

O código implementa a leitura de dados do acelerômetro MPU6050 utilizando comunicação I2C.

A inicialização da interface I2C é realizada na função ``i2c_init()`` presente em ``mpu6050.c``, configurando os pinos GPIO 21 (SDA) e GPIO 22 (SCL) com frequência de 100 kHz.

Após isso, a função ``mpu6050_init()`` retira o sensor do modo de sleep escrevendo no registrador ``MPU6050_PWR_MGMT_1`` (0x6B), permitindo que o módulo comece a transmitir dados.

A leitura dos valores do acelerômetro é feita pela função ``mpu6050_read_accel()``, que acessa o registrador ``MPU6050_ACCEL_XOUT_H`` (0x3B) e lê os 6 bytes correspondentes aos eixos X, Y e Z.

Os valores brutos de 16 bits são convertidos para unidade de gravidade (g) utilizando:

::

    out->ax = ax / 16384.0;
    out->ay = ay / 16384.0;
    out->az = az / 16384.0;

Essa conversão é baseada na configuração padrão do sensor em ±2g.

A função ``teste_start()`` inicializa o MPU6050 e cria uma tarefa FreeRTOS chamada ``mpu_task()``, responsável por realizar leituras contínuas do sensor e imprimir os valores no monitor serial a cada 500 ms:

::

    printf("AX: %.2f | AY: %.2f | AZ: %.2f\n",
           data.ax,
           data.ay,
           data.az);

Dessa forma, é possível validar o funcionamento do acelerômetro e observar em tempo real as variações dos três eixos conforme a movimentação do sensor.

Referências
---------
- `Tutorial I2C ESP32 <https://microcontrollerslab.com/esp32-i2c-communication-tutorial-arduino-ide>`_
- `Documentação Espressif <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_
- `Documentação I2C utilizado para programar o MPU6050 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_
- `Exemplos de programação ESP32 Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_
- `ESP32 as I2C Master <https://www.youtube.com/watch?v=Snp6iTu1R7E>`_

