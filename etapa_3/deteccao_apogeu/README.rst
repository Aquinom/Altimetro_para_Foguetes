Telemetria para Foguetes com ESP32
==================================

.. contents::
   :local:
   :depth: 2

Objetivos da Etapa
------------------

Esta etapa teve como objetivo desenvolver e validar a primeira versão do algoritmo responsável pela identificação automática do apogeu do foguete. Para isso, foram implementados os módulos de aquisição dos sensores, estimação da altitude, filtragem das leituras, cálculo da velocidade vertical e detecção do ponto de altitude máxima.

Além do algoritmo de detecção, foi desenvolvida a estrutura responsável pela aquisição e transmissão dos dados pela interface serial (UART), permitindo acompanhar o comportamento do sistema durante os testes em bancada e fornecendo uma base para a integração com as próximas etapas do firmware.

Visão Geral
-----------

Este módulo implementa a primeira versão do sistema de aquisição e processamento dos dados de voo do foguete. O objetivo é realizar a leitura dos sensores, estimar a altitude, calcular a velocidade vertical e detectar automaticamente o apogeu, disponibilizando essas informações para as próximas etapas do firmware.

Nesta versão, a transmissão dos dados é realizada por meio da interface serial (UART), permitindo acompanhar o comportamento do algoritmo em tempo real durante os testes em bancada. O armazenamento permanente dos dados por NVS será implementado na próxima etapa do projeto.

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

Sensores utilizados neste teste:

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

O módulo ``flight_controller`` centraliza toda a lógica de voo do sistema. Ele é responsável por integrar as leituras dos sensores, estimar as variáveis de interesse, detectar eventos como o apogeu e disponibilizar essas informações para os demais módulos do firmware.

Funcionamento do Sistema
------------------------

O firmware foi desenvolvido utilizando o framework ESP-IDF [1] e o sistema operacional FreeRTOS [2].

O processamento das informações ocorre continuamente conforme o fluxo apresentado abaixo.

::

               BMP280                 MPU6050
                  │                      │
                  │                      │
                  └──────────┬───────────┘
                             │
                             ▼
                Aquisição dos sensores
                             │
                             ▼
                Calibração inicial
                             │
                             ▼
                Filtragem das medições
                             │
                             ▼
                Estimativa da altitude (utilizando pressão do bar)
                             │
                             ▼
                Estimativa da velocidade vertical (utilizando uma fusão de sensores)
                             │
                             ▼
                Verificação dos critérios de apogeu
                             │
                             ▼
                Envio das informações (UART)

Esse fluxo representa todas as etapas executadas pelo firmware durante seu funcionamento, desde a leitura dos sensores até a disponibilização das informações para acompanhamento em tempo real.

Calibração dos Sensores
-----------------------

Antes do início da operação, o firmware realiza a calibração dos sensores para estabelecer os valores de referência utilizados durante o voo.

No caso do BMP280, a calibração consiste na obtenção da pressão atmosférica de referência enquanto o foguete permanece em repouso na base de lançamento. Durante esse período são adquiridas 100 amostras consecutivas da pressão atmosférica, sendo calculada a média dessas medições. Esse valor é utilizado como pressão de referência para a aplicação da equação barométrica, permitindo que a altitude seja estimada em relação ao ponto de lançamento, em vez da altitude em relação ao nível do mar.

Para o MPU6050, a calibração é realizada mantendo o foguete completamente parado durante a inicialização do sistema. Nessa condição, 200 amostras da aceleração são coletadas para determinar o offset presente em cada eixo do acelerômetro. Os offsets calculados são posteriormente subtraídos das leituras durante a execução do firmware, reduzindo erros sistemáticos provocados por pequenas imperfeições do sensor.

Após a calibração, todos os cálculos de altitude, velocidade vertical e detecção de apogeu passam a utilizar esses valores de referência. Dessa forma, pequenas diferenças entre sensores ou variações das condições ambientais no momento do lançamento exercem menor influência sobre o desempenho do algoritmo.

Aquisição dos Sensores
----------------------

A comunicação entre o ESP32 e os sensores é realizada pelo barramento I²C utilizando os drivers disponibilizados pela ESP-IDF [3].

O BMP280 fornece medições de pressão atmosférica e temperatura [5], enquanto o MPU6050 fornece aceleração nos três eixos [6].

A altitude é estimada a partir da pressão atmosférica utilizando a equação barométrica e uma pressão de referência obtida durante a calibração inicial.

Resumo dos sensores utilizados:

+-----------+-----------------------------+----------------------------------+
| Sensor    | Grandeza medida             | Utilização no algoritmo          |
+===========+=============================+==================================+
| BMP280    | Pressão e temperatura       | Estimativa da altitude           |
+-----------+-----------------------------+----------------------------------+
| MPU6050   | Aceleração nos três eixos   | Estimativa da velocidade         |
+-----------+-----------------------------+----------------------------------+

Filtragem das Leituras
----------------------

As medições do barômetro e acelerômetro apresentam pequenas oscilações provocadas pelo ruído eletrônico dos sensores, vibrações estruturais do foguete e pequenas variações instantâneas da pressão atmosférica.

Para reduzir esse efeito foi implementado um filtro de média móvel utilizando uma janela de quinze amostras.

Esse filtro suaviza as medições sem comprometer significativamente o tempo de resposta necessário para detectar o apogeu, reduzindo a ocorrência de falsas detecções.

Estimativa da Velocidade Vertical
---------------------------------

Inicialmente, a velocidade vertial é obtida pela integração da aceleração medida pelo MPU6050.

Em seguida, essa estimativa é combinada com a velocidade calculada a partir da taxa de variação da altitude barométrica.

Essa fusão reduz o efeito do ruído presente em cada sensor individualmente, aproveitando a resposta rápida do acelerômetro e a maior estabilidade da altitude estimada pelo barômetro.

Os coeficientes utilizados na combinação (0,35 para o acelerômetro e 0,65 para o barômetro) foram definidos experimentalmente durante os testes em bancada e poderão ser ajustados após os primeiros ensaios em voo. Foi atribuído um peso maior às estimativas provenientes do barômetro devido à maior estabilidade das medições de altitude, enquanto o acelerômetro está mais sujeito aos efeitos de ruídos, vibrações e ao acúmulo de erros decorrente da integração da aceleração.

Parâmetros do Algoritmo
-----------------------

+-----------------------------+-------------------------------+
| Parâmetro                   | Valor                         |
+=============================+===============================+
| Janela da média móvel       | 15 amostras                   |
+-----------------------------+-------------------------------+
| Peso do acelerômetro        | 0,35                          |
+-----------------------------+-------------------------------+
| Peso do barômetro           | 0,65                          |
+-----------------------------+-------------------------------+
| Comunicação                 | I²C                           |
+-----------------------------+-------------------------------+
| Sistema operacional         | FreeRTOS                      |
+-----------------------------+-------------------------------+

Detecção de Apogeu
------------------

Após o processamento das leituras, o firmware verifica continuamente se o foguete atingiu o ponto de altitude máxima.

A detecção ocorre somente quando três condições são satisfeitas simultaneamente:

* altitude acima de um limite mínimo;
* velocidade vertical negativa, indicando o início da descida;
* aceleração vertical compatível com a transição entre a fase de subida e a fase de descida.

A utilização conjunta dessas condições reduz significativamente falsas detecções provocadas por oscilações momentâneas dos sensores.

Os critérios utilizados pelo algoritmo são resumidos na tabela a seguir.

+--------------------------------------+---------------------------------------------+
| Critério                             | Finalidade                                  |
+======================================+=============================================+
| Altitude acima do limite mínimo      | Evita detecção durante a inicialização      |
+--------------------------------------+---------------------------------------------+
| Velocidade vertical negativa         | Indica o início da descida                  |
+--------------------------------------+---------------------------------------------+
| Aceleração compatível com a descida  | Confirma a mudança de fase do voo           |
+--------------------------------------+---------------------------------------------+

De forma simplificada, o algoritmo executa continuamente o seguinte procedimento:

::

    Enquanto o sistema estiver ativo

        Ler os sensores

        Filtrar as medições

        Estimar a altitude

        Estimar a velocidade vertical

        Se:

            altitude > limite mínimo

            velocidade < 0

            aceleração indicar descida

        Então:

            Detectar apogeu

Arquitetura do Firmware
-----------------------

O sistema foi dividido em duas tarefas do FreeRTOS.

A primeira é responsável pela aquisição e processamento das informações provenientes dos sensores.

A segunda realiza exclusivamente a transmissão das informações processadas para o monitor serial.

Essa separação evita que atrasos na comunicação serial interfiram na frequência de aquisição dos sensores, preservando o desempenho do algoritmo de detecção de apogeu.

As duas tarefas compartilham uma estrutura comum contendo as informações de voo.

Para evitar condições de corrida durante esse acesso compartilhado é utilizado um mutex, garantindo acesso exclusivo aos dados durante operações críticas.

Log de Funcionamento
--------------------

Para validar o algoritmo de detecção de apogeu foi realizado um teste em bancada utilizando um BMP280 e um MPU6050.

Como não era possível reproduzir um voo real em laboratório, foi definida uma altura aproximada de 50 cm como referência para representar o apogeu.

A **Figura 1** apresenta o monitor serial durante o ensaio.

Quando a altura máxima é atingida, o firmware identifica corretamente o apogeu, registra a mensagem correspondente e armazena o maior valor de altitude medido, que será utilizado nas próximas etapas da máquina de estados.

.. image:: ../images/apogeu_detection_test.jpg
   :alt: Resultado do teste de detecção de apogeu.
   :align: center
   :width: 600px

**Figura 1.** Resultado do algoritmo de detecção de apogeu durante teste em bancada.

Os resultados demonstram que o algoritmo foi capaz de identificar corretamente o ponto de altitude máxima nas condições simuladas, validando a abordagem adotada antes da realização de testes em voo.

Limitações Atuais
-----------------

A implementação atual foi validada apenas em ensaios realizados em bancada, utilizando deslocamentos controlados para simular a trajetória do foguete e sem os outros 2x barômetros que serão utilizados no projeto final.

Os limiares empregados pelo algoritmo foram definidos experimentalmente e poderão ser refinados após os primeiros testes em voo, quando será possível avaliar o comportamento do sistema em condições reais de operação.

Referências
-----------

[1] `Documentação ESP-IDF <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_

[2] `Documentação FreeRTOS <https://www.freertos.org/Documentation/RTOS_book.html>`_

[3] `Documentação I2C ESP32 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_

[4] `Exemplos ESP-IDF Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_

[5] `Datasheet BMP280 <https://www.bosch-sensortec.com/products/environmental-sensors/pressure-sensors/bmp280/>`_

[6] `Datasheet MPU6050 <https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/>`_

