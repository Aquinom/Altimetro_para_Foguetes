Telemetria para Foguetes com ESP32
==================================

Sistema de telemetria embarcada desenvolvido utilizando ESP32, ESP-IDF e FreeRTOS para aquisição, processamento e transmissão de dados em tempo real durante o voo de um foguete.

Conexões de hardware
--------------------

ESP32 DevKit:

- GPIO 21 → SDA (BMP280 e MPU6050)
- GPIO 22 → SCL (BMP280 e MPU6050)
- 3V3 → VCC (BMP280 e MPU6050)
- 3V3 → CSB (BMP280)
- GND → GND (BMP280 e MPU6050)
- GND → SDO (BMP280)
- GND → AD0 (MPU6050)

Sensores utilizados:

- 1X BMP280
- 1X MPU6050

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

O firmware foi desenvolvido utilizando o framework ESP-IDF [1] e o sistema operacional FreeRTOS [2], permitindo a execução concorrente das tarefas responsáveis pela aquisição dos sensores e pela transmissão da telemetria.

A comunicação entre o microcontrolador e os sensores é realizada por meio do barramento I2C, conforme a API disponibilizada pela Espressif [3].

Durante a execução do sistema são obtidos continuamente os seguintes parâmetros:

- Pressão atmosférica;
- Altitude;
- Temperatura;
- Aceleração nos três eixos;
- Velocidade vertical estimada.

A arquitetura do firmware foi organizada em módulos independentes, separando os drivers dos sensores da lógica principal da aplicação. Essa organização facilita a manutenção do código, a reutilização dos drivers e a integração de novos dispositivos.

A leitura dos sensores é realizada em uma task dedicada do FreeRTOS:

::

    xTaskCreatePinnedToCore(
        sensor_task,
        "sensor_task",
        4096,
        NULL,
        5,
        NULL,
        1);

Paralelamente, uma segunda task é responsável exclusivamente pela transmissão dos dados processados via interface serial:

::

    xTaskCreatePinnedToCore(
        telemetry_task,
        "telemetry_task",
        4096,
        NULL,
        1,
        NULL,
        0);

Como as duas tarefas acessam informações compartilhadas, foi utilizada sincronização por meio de mutex, evitando condições de corrida entre as tasks [2].

::

    xSemaphoreTake(telemetry_mutex, portMAX_DELAY);

    telemetry.altitude = altitude;

    xSemaphoreGive(telemetry_mutex);

Para reduzir oscilações provenientes das vibrações durante o voo, os dados de altitude passam por um filtro de média móvel com 15 amostras.

::

    #define MOVING_AVG_SIZE 15

A atualização do filtro ocorre continuamente durante a aquisição dos dados.

::

    altitude = ma_update(&alt_filter, altitude_raw);

A velocidade vertical é estimada combinando informações provenientes do acelerômetro e do barômetro, aumentando a robustez da estimativa quando comparada ao uso isolado de apenas um sensor.

::

    velocity += accel * dt * 0.25f;

    velocity =
        (velocity * 0.35f) +
        (vel_baro * 0.65f);

O firmware também implementa um algoritmo para detecção do apogeu do voo. A decisão é baseada na combinação das leituras dos sensores, considerando simultaneamente altitude mínima, velocidade vertical negativa e aceleração compatível com o início da descida.

::

    if (
        altitude_valid &&
        descending &&
        accel_negative)
    {
        apogee_detected = true;

        ESP_LOGW(TAG, "APOGEU DETECTADO");
    }

Após o processamento, os dados são enviados continuamente ao monitor serial para acompanhamento do comportamento do sistema em tempo real.

::

    ESP_LOGI(
        TAG,
        "ALT: %.2f m | VEL: %.2f m/s | ACC: %.3f g",
        local.altitude,
        local.velocity,
        local.acceleration);

Log de Funcionamento
--------------------

Para validar o algoritmo de detecção do apogeu foi realizado um teste em bancada utilizando o conjunto completo de sensores.

Como não era possível reproduzir um voo real em ambiente de laboratório, foi definida uma altura de aproximadamente **50 cm** como referência para simular o apogeu. Durante o ensaio, o sistema monitorou continuamente a altitude estimada e identificou corretamente o instante em que a altura máxima foi atingida.

A **Figura 1** apresenta o log obtido durante o teste. Observa-se que, ao atingir a altura configurada, o firmware registra a mensagem de detecção do apogeu e armazena o maior valor de altitude medido (*max*), mantendo esse valor disponível para utilização nas próximas etapas da máquina de estados.

.. image:: ../images/apogeu_detection_test.jpg
   :alt: Apogeu Detection
   :align: center
   :width: 600px

**Figura 1.** Log da execução do algoritmo de detecção de apogeu durante teste em bancada.

O resultado obtido demonstra que o algoritmo foi capaz de detectar corretamente o ponto de altitude máxima dentro das condições simuladas, validando sua integração com o restante do firmware antes da realização de testes em voo.

Referências
-----------

[1] `Documentação ESP-IDF <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_

[2] `Documentação FreeRTOS <https://www.freertos.org/Documentation/RTOS_book.html>`_

[3] `Documentação I2C ESP32 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_

[4] `Exemplos ESP-IDF Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_

[5] `Datasheet BMP280 <https://www.bosch-sensortec.com/products/environmental-sensors/pressure-sensors/bmp280/>`_

[6] `Datasheet MPU6050 <https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/>`_
