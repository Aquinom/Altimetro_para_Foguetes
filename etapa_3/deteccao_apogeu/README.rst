Telemetria para Foguetes com ESP32
==================================

Este módulo implementa a primeira versão do sistema de aquisição e processamento dos dados de voo do foguete. O objetivo é realizar a leitura dos sensores, estimar a altitude, calcular a velocidade vertical e detectar automaticamente o apogeu, disponibilizando essas informações para as próximas etapas do firmware.

Nesta versão, a transmissão dos dados é realizada por meio da interface serial (UART), permitindo acompanhar o comportamento do algoritmo em tempo real durante os testes em bancada. O armazenamento permanente dos dados e a telemetria por rádio serão implementados nas próximas etapas do projeto.

Conexões de Hardware
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

- 1 × BMP280
- 1 × MPU6050

Estrutura do Projeto
--------------------

::

    main/
     ├── main.c
     ├── flight_controller.c
     ├── flight_controller.h
     └── drivers/
          ├── bmp280/
          ├── mpu6050/
          └── i2c/

O módulo ``flight_controller`` centraliza a lógica de voo do sistema. Ele é responsável por integrar as leituras dos sensores, estimar as variáveis de interesse, detectar eventos como o apogeu e disponibilizar essas informações para o restante do firmware.

Funcionamento do Sistema
------------------------

O firmware foi desenvolvido utilizando o framework ESP-IDF [1] e o sistema operacional FreeRTOS [2].

O sistema realiza continuamente as seguintes etapas de processamento:

::

    BMP280 + MPU6050
            │
            ▼
    Aquisição dos dados
            │
            ▼
    Filtro de média móvel
            │
            ▼
    Estimativa da altitude
            │
            ▼
    Estimativa da velocidade
            │
            ▼
    Detecção do apogeu
            │
            ▼
    Envio pela UART

Essa sequência representa o fluxo completo de processamento executado durante o funcionamento do firmware.

Calibração dos Sensores
-----------------------

Antes do início da operação, o firmware realiza a calibração dos sensores para estabelecer os valores de referência utilizados durante o voo.

No caso do BMP280, a calibração consiste na obtenção da pressão atmosférica de referência enquanto o foguete permanece em repouso na base de lançamento. Durante esse período são adquiridas diversas amostras consecutivas da pressão atmosférica, sendo calculada a média dessas medições. Esse valor é utilizado como pressão de referência para a aplicação da equação barométrica, permitindo que a altitude seja estimada em relação ao ponto de lançamento, em vez da altitude em relação ao nível do mar.

Para o MPU6050, a calibração é realizada mantendo o foguete completamente parado durante a inicialização do sistema. Nessa condição, diversas amostras da aceleração são coletadas para determinar o offset presente em cada eixo do acelerômetro. Os offsets calculados são posteriormente subtraídos das leituras durante a execução do firmware, reduzindo erros sistemáticos provocados por pequenas imperfeições do sensor.

Após a calibração, todos os cálculos de altitude, velocidade vertical e detecção de apogeu passam a utilizar esses valores de referência. Dessa forma, pequenas diferenças entre sensores ou variações das condições ambientais no momento do lançamento têm menor influência sobre o desempenho do algoritmo.

Aquisição dos Sensores
----------------------

A comunicação entre o ESP32 e os sensores é realizada pelo barramento I²C utilizando os drivers disponibilizados pela ESP-IDF [3].

O BMP280 fornece medições de pressão atmosférica e temperatura [5], enquanto o MPU6050 fornece aceleração nos três eixos [6].

A altitude não é medida diretamente pelo sistema. Ela é estimada a partir da pressão atmosférica utilizando a equação barométrica e uma pressão de referência obtida antes do lançamento.

Filtragem das Leituras
----------------------

As medições do barômetro apresentam pequenas oscilações provocadas pelo ruído eletrônico dos sensores e pelas vibrações do foguete.

Para reduzir esse efeito foi implementado um filtro de média móvel utilizando uma janela de quinze amostras.

Esse filtro suaviza as medições sem comprometer significativamente o tempo de resposta necessário para detectar o apogeu.

Estimativa da Velocidade Vertical
---------------------------------

A velocidade vertical também não é medida diretamente.

Inicialmente ela é obtida por integração da aceleração medida pelo MPU6050.

Em seguida essa estimativa é combinada com a velocidade calculada a partir da variação da altitude barométrica.

Essa fusão reduz o efeito do ruído presente em cada sensor individualmente.

Os coeficientes utilizados na combinação (0,35 para o acelerômetro e 0,65 para o barômetro) foram definidos experimentalmente durante os testes em bancada e poderão ser ajustados nas próximas etapas do projeto.

Detecção de Apogeu
------------------

Após o processamento das leituras, o firmware verifica continuamente se o foguete atingiu o ponto de altitude máxima.

A detecção ocorre quando três condições são satisfeitas simultaneamente:

* altitude acima de um limite mínimo;
* velocidade vertical negativa, indicando início da descida;
* aceleração vertical compatível com o término da fase de subida.

A utilização conjunta dessas condições reduz falsas detecções provocadas por oscilações dos sensores.

Arquitetura do Firmware
-----------------------

O sistema foi dividido em duas tarefas do FreeRTOS.

A primeira é responsável pela aquisição e processamento dos sensores.

A segunda realiza apenas o envio das informações processadas para o monitor serial.

Essa separação impede que atrasos na transmissão serial interfiram na frequência de aquisição dos sensores.

As duas tarefas compartilham uma estrutura comum de telemetria.

Para evitar condições de corrida durante esse acesso compartilhado é utilizado um mutex, garantindo que apenas uma tarefa modifique os dados por vez.

Log de Funcionamento
--------------------

Para validar o algoritmo de detecção de apogeu foi realizado um teste em bancada utilizando um BMP280 e um MPU6050.

Como não era possível reproduzir um voo real em laboratório, foi definida uma altura aproximada de 50 cm como referência para representar o apogeu.

A **Figura 1** apresenta o monitor serial durante o ensaio.

Quando a altura máxima é atingida, o firmware identifica corretamente o apogeu, registra a mensagem correspondente e armazena o maior valor de altitude medido, que será utilizado nas próximas etapas da máquina de estados.

.. image:: ../images/apogeu_detection_test.jpg
   :alt: Apogeu Detection
   :align: center
   :width: 600px

**Figura 1.** Resultado do algoritmo de detecção de apogeu durante teste em bancada.

Os resultados demonstram que o algoritmo foi capaz de identificar corretamente o ponto de altitude máxima nas condições simuladas, validando a abordagem adotada antes da realização de testes em voo.

Referências
-----------

[1] `Documentação ESP-IDF <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_

[2] `Documentação FreeRTOS <https://www.freertos.org/Documentation/RTOS_book.html>`_

[3] `Documentação I2C ESP32 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_

[4] `Exemplos ESP-IDF Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_

[5] `Datasheet BMP280 <https://www.bosch-sensortec.com/products/environmental-sensors/pressure-sensors/bmp280/>`_

[6] `Datasheet MPU6050 <https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/>`_
