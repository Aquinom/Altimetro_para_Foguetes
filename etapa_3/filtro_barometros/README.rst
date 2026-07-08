Fusão de Dados de Três Barômetros
=================================

.. contents::
   :local:
   :depth: 2

Objetivos da Etapa
------------------

Esta etapa teve como objetivo desenvolver um sistema de aquisição e fusão de dados utilizando três sensores barométricos BMP280. O emprego de múltiplos sensores busca aumentar a confiabilidade das medições de pressão atmosférica e altitude, reduzindo a influência de ruídos e de eventuais leituras inconsistentes provenientes de um único sensor.

Para isso, foi implementado um algoritmo composto pelas etapas de aquisição das leituras, filtragem individual dos sensores, cálculo da mediana, atribuição dinâmica de pesos e fusão das informações, produzindo uma única estimativa de pressão utilizada pelo restante do firmware.

Visão Geral
-----------

O módulo realiza continuamente a leitura dos três sensores BMP280 conectados ao ESP32 por meio do barramento I²C.

Cada sensor fornece medições de pressão atmosférica e temperatura. Em seguida, as leituras passam por um processo de filtragem e fusão, gerando uma estimativa única da pressão atmosférica utilizada para calcular a altitude relativa do foguete.

Essa abordagem aumenta a robustez do sistema, reduzindo a influência de ruídos eletrônicos, pequenas diferenças entre sensores e eventuais leituras discrepantes.

Conexões de Hardware
--------------------

ESP32 DevKit:

- GPIO 21 → SDA (BMP280_0 e BMP280_1)
- GPIO 22 → SCL (BMP280_0 e BMP280_1)
- GPIO 19 → SDA (BMP280_2)
- GPIO 18 → SCL (BMP280_2)
- 3V3 → VCC (BMP280_0, BMP280_1 e BMP280_2)
- GND → GND (BMP280_0, BMP280_1 e BMP280_2)
- GND → SDO (BMP280_2)
- SDO desconectado (BMP280_0 e BMP280_1)

Sensores utilizados:

- 3 × BMP280

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

O módulo ``baro_fusion`` concentra toda a lógica de processamento das leituras dos sensores, sendo responsável pela filtragem, fusão das informações e disponibilização da altitude para o restante do firmware.

Funcionamento do Algoritmo
--------------------------

O processamento executado pelo sistema pode ser resumido pelo fluxo abaixo.

::

          BMP280_0      BMP280_1      BMP280_2
               │            │             │
               └──────┬─────┴─────────────┘
                      │
                      ▼
            Aquisição das leituras
                      │
                      ▼
            Filtro Exponencial (EMA)
                      │
                      ▼
            Cálculo da Mediana
                      │
                      ▼
            Cálculo dos desvios individuais
                      │
                      ▼
            Atribuição dinâmica de pesos
                      │
                      ▼
            Média ponderada das leituras
                      │
                      ▼
            Conversão para altitude
                      │
                      ▼
            Cálculo da altitude relativa

Cada etapa possui uma função específica dentro do algoritmo, contribuindo para aumentar a estabilidade das medições utilizadas pelo sistema.

Aquisição dos Sensores
----------------------

A comunicação entre o ESP32 e os sensores BMP280 é realizada por meio do barramento I²C utilizando os drivers disponibilizados pela ESP-IDF [1].

Cada sensor realiza medições independentes da pressão atmosférica e da temperatura. Essas informações são coletadas continuamente e encaminhadas ao módulo de fusão dos sensores.

A utilização de três sensores independentes permite reduzir a influência de falhas individuais e aumentar a confiabilidade das estimativas obtidas.

Filtragem Individual das Leituras
---------------------------------

Antes da fusão das informações, cada sensor passa individualmente por um filtro exponencial (Exponential Moving Average – EMA).

Esse filtro reduz pequenas oscilações presentes nas medições de pressão, atribuindo maior peso às leituras anteriores e suavizando variações instantâneas provocadas por ruídos eletrônicos.

A utilização da EMA permite melhorar a estabilidade das medições sem aumentar significativamente o atraso da resposta do sistema.

Cálculo da Mediana
------------------

Após a filtragem, é calculada a mediana das três leituras de pressão.

A mediana é utilizada como referência por apresentar maior robustez à presença de valores discrepantes (outliers). Caso um dos sensores apresente uma leitura incorreta, a mediana continua representando adequadamente o comportamento dos demais sensores.

Essa etapa serve como base para a atribuição dos pesos utilizados durante a fusão dos dados.

Atribuição Dinâmica de Pesos
----------------------------

Cada sensor recebe um peso proporcional à diferença entre sua leitura e a mediana calculada anteriormente.

Sensores cujas medições permanecem próximas da mediana recebem maior peso, enquanto sensores que apresentam diferenças elevadas têm sua influência reduzida.

Os pesos utilizados pelo algoritmo são apresentados na tabela abaixo.

+-------------------------------------+----------------+
| Diferença para a mediana            | Peso atribuído |
+=====================================+================+
| Menor que 20 Pa                     | 100            |
+-------------------------------------+----------------+
| Entre 20 Pa e 49 Pa                 | 70             |
+-------------------------------------+----------------+
| Entre 50 Pa e 99 Pa                 | 30             |
+-------------------------------------+----------------+
| Igual ou superior a 100 Pa          | 0              |
+-------------------------------------+----------------+

Essa estratégia reduz automaticamente a influência de sensores que apresentem leituras inconsistentes, aumentando a confiabilidade da estimativa final.

Fusão das Leituras
------------------

Após a definição dos pesos, é realizada uma média ponderada das três medições de pressão.

Sensores considerados mais confiáveis contribuem com maior influência para o resultado final, enquanto sensores classificados como discrepantes possuem participação reduzida ou completamente descartada.

A pressão resultante representa a estimativa utilizada pelo restante do firmware para o cálculo da altitude.

Estimativa da Altitude
----------------------

A altitude não é medida diretamente pelos sensores BMP280.

Ela é estimada a partir da pressão atmosférica utilizando a equação barométrica.

Antes da operação do sistema é realizada uma rotina de calibração responsável por determinar a altitude correspondente ao ponto de lançamento. Durante essa etapa são adquiridas diversas medições consecutivas, sendo calculada sua média.

Esse valor é armazenado como referência e passa a representar a altitude zero do sistema. Durante a operação, todas as estimativas de altitude são apresentadas em relação a essa referência.

Arquitetura do Firmware
-----------------------

A aquisição e o processamento das leituras são executados por uma tarefa dedicada do FreeRTOS.

Essa tarefa realiza continuamente:

* leitura dos três sensores;
* filtragem individual das medições;
* cálculo da mediana;
* atribuição dinâmica dos pesos;
* fusão das leituras;
* cálculo da altitude relativa.

A organização em uma tarefa dedicada simplifica o processamento das informações e facilita a integração com os demais módulos do firmware.

Resultados Obtidos
------------------

O algoritmo desenvolvido permite combinar automaticamente as informações provenientes dos três sensores BMP280, reduzindo a influência de ruídos e de leituras discrepantes.

A utilização conjunta do filtro exponencial, da mediana e da média ponderada fornece estimativas mais estáveis de pressão e altitude quando comparadas ao uso de um único sensor.

Essa arquitetura estabelece uma base confiável para os algoritmos de navegação e detecção de eventos desenvolvidos nas etapas seguintes do projeto.

Limitações Atuais
-----------------

Nesta etapa, o algoritmo foi validado em ensaios realizados em bancada.

Os limites utilizados para a atribuição dos pesos foram definidos experimentalmente e poderão ser refinados após a realização de testes em voo, quando será possível avaliar o comportamento do sistema sob condições reais de operação.

Referências
-----------

[1] `Documentação ESP-IDF <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_

[2] `Documentação FreeRTOS <https://www.freertos.org/Documentation/RTOS_book.html>`_

[3] `Documentação I2C ESP32 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_

[4] `Exemplos ESP-IDF Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_

[5] `Datasheet BMP280 <https://www.bosch-sensortec.com/products/environmental-sensors/pressure-sensors/bmp280/>`_
