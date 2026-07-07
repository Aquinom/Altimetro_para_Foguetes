Etapa 3
#######

.. contents::
   :local:
   :depth: 2


Visão geral
***********

A Etapa 3 teve como objetivo desenvolver os primeiros algoritmos de processamento dos dados dos sensores, revisar o projeto eletrônico da placa e realizar a integração inicial entre os módulos de hardware e software.

As atividades desenvolvidas nesta etapa foram:

* Implementação da estimativa inicial de altitude com filtragem;
* Desenvolvimento do algoritmo de detecção de lançamento e apogeu;
* Revisão do esquemático e do layout preliminar da PCB;
* Primeira integração entre os módulos do sistema.

Essas atividades representam a transição entre os testes individuais dos periféricos, realizados na etapa anterior, e o desenvolvimento de um sistema embarcado integrado capaz de interpretar os dados dos sensores e tomar decisões durante o voo.

Desenvolvimento
***************

O primeiro algoritmo implementado nesta etapa foi responsável pela estimativa da altitude do foguete.

A altitude não é medida diretamente pelos sensores, sendo estimada a partir da pressão atmosférica obtida pelos três barômetros utilizando a equação barométrica. Para isso, a pressão medida antes do lançamento é utilizada como pressão de referência, permitindo calcular a altitude relativa durante o voo.

Como as leituras dos sensores estão sujeitas a ruídos e pequenas oscilações, foi implementado um filtro simples baseado em média móvel e atribuição de pesos aos diferentes barômetros. Além disso, leituras discrepantes são descartadas antes da composição da estimativa final, aumentando a confiabilidade do sistema.

Após a implementação da estimativa de altitude, iniciou-se o desenvolvimento do algoritmo de detecção do lançamento e do apogeu do foguete.

Esse algoritmo combina informações provenientes do acelerômetro e dos barômetros para identificar automaticamente as diferentes fases do voo. A utilização conjunta desses sensores aumenta a robustez da detecção quando comparada ao uso de apenas um sensor.

Os detalhes da implementação podem ser encontrados na documentação específica da **Detecção de Apogeu**.

Paralelamente ao desenvolvimento do firmware, foi realizada uma revisão completa do projeto eletrônico da placa.

Durante essa revisão foi identificado que o servomotor SG90 não pode ser alimentado diretamente pela bateria LiPo 2S, cuja tensão varia aproximadamente entre 6,6 V e 8,4 V durante sua operação. Assim, tornou-se necessária a inclusão de um circuito de conversão de tensão para garantir que o servo opere dentro da faixa especificada pelo fabricante.

Também foram realizados ajustes no esquemático, revisão dos footprints dos módulos utilizados e adequações no layout da PCB antes da fabricação da primeira versão da placa.

Detecção de Apogeu
==================

Nesta etapa foi implementada a primeira versão do algoritmo de detecção de apogeu.

O algoritmo utiliza informações provenientes do acelerômetro e dos barômetros para identificar automaticamente o instante em que o foguete atinge sua altitude máxima. Essa informação será utilizada nas etapas seguintes para o acionamento do sistema de recuperação.

A documentação completa da implementação pode ser acessada no link abaixo.

`Detecção de Apogeu <deteccao_apogeu>`_

Filtro dos Barômetros
=====================

Foi desenvolvido um algoritmo de filtragem para reduzir oscilações nas leituras dos barômetros e melhorar a estimativa de altitude.

O filtro realiza a combinação das medições provenientes dos três sensores, reduzindo a influência de leituras discrepantes e aumentando a estabilidade da altitude estimada.

Os detalhes da implementação encontram-se na documentação específica.

`Filtro dos Barômetros <filtro_barometros>`_

Esquemático e Layout da PCB
===========================

Durante esta etapa foi realizada uma revisão completa do projeto eletrônico da placa.

Foram efetuadas correções no esquemático, ajustes de layout e modificações no circuito de alimentação, incluindo a adição de um conversor de tensão para alimentação adequada do servomotor.

O projeto completo da PCB, incluindo arquivos do KiCad, esquemáticos em PDF e imagens do layout, encontra-se disponível no link abaixo.

`Projeto da PCB <PCB>`_

Integração V1
=============

A primeira etapa de integração teve como objetivo validar a comunicação entre os módulos de software desenvolvidos até o momento.

Nessa versão foram integrados o algoritmo de estimativa de altitude, o algoritmo de detecção de apogeu e a máquina de estados desenvolvida com o Itemis Create, utilizando o FreeRTOS como sistema operacional de tempo real para gerenciamento das tarefas do sistema.

Como os sensores físicos ainda não estavam completamente integrados, foram utilizados dados simulados que reproduzem o comportamento esperado durante um voo, permitindo validar a comunicação entre os módulos e a lógica geral do firmware.

Os códigos utilizados durante os testes de integração podem ser acessados no link abaixo.

`Integração V1 <Integracao_V1>`_

Referências
***********

[1] `Tutorial I2C ESP32 <https://microcontrollerslab.com/esp32-i2c-communication-tutorial-arduino-ide>`_

[2] `Tutorial Itemis Create <https://www.itemis.com/en/products/itemis-create/documentation/tutorials>`_

[3] `Documentação Espressif <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_

[4] `Documentação LEDC (ESP-IDF) <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html>`_

[5] `Documentação I2C (ESP-IDF) <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_

[6] `Exemplos ESP-IDF Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_

[7] `ESP32 as I2C Master <https://www.youtube.com/watch?v=Snp6iTu1R7E>`_
