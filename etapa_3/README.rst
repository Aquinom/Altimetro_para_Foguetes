Etapa 3
#######

.. contents::
   :local:
   :depth: 2


Visão Geral
***********

A Etapa 3 concentrou-se no desenvolvimento dos principais módulos de software do altímetro e na revisão do projeto eletrônico da placa, estabelecendo a base necessária para a integração completa do sistema nas etapas seguintes.

Até a conclusão da Etapa 2, os periféricos haviam sido validados individualmente. Nesta etapa, o foco passou a ser o desenvolvimento de algoritmos capazes de processar as informações provenientes dos sensores, revisar o hardware e estruturar a arquitetura inicial do firmware.

As principais atividades desenvolvidas foram:

* desenvolvimento do algoritmo de detecção de apogeu;
* desenvolvimento do algoritmo de filtragem das leituras dos três barômetros;
* revisão do esquemático e do layout preliminar da placa de circuito impresso (PCB);
* desenvolvimento da primeira versão da arquitetura de integração utilizando máquina de estados.

Cada uma dessas atividades foi desenvolvida e validada individualmente, permitindo verificar o funcionamento de cada módulo antes da integração completa do sistema, prevista para a próxima etapa do projeto.

Desenvolvimento
***************

Nesta etapa, o projeto foi dividido em quatro frentes de desenvolvimento independentes, cada uma responsável por validar uma funcionalidade específica do altímetro. Essa estratégia permitiu que os algoritmos e o projeto eletrônico fossem desenvolvidos separadamente, reduzindo a complexidade durante a fase de implementação e facilitando a validação individual de cada módulo.

A Tabela 1 apresenta um resumo das atividades desenvolvidas nesta etapa.

+----------------------+------------------------------------------------------+
| Subprojeto           | Objetivo                                             |
+======================+======================================================+
| Detecção de Apogeu   | Desenvolver e validar o algoritmo responsável pela   |
|                      | identificação do apogeu do foguete.                  |
+----------------------+------------------------------------------------------+
| Filtro dos           | Desenvolver o algoritmo de filtragem das leituras    |
| Barômetros           | provenientes dos três sensores BMP280.               |
+----------------------+------------------------------------------------------+
| Projeto da PCB       | Revisar o esquemático, layout e circuito de          |
|                      | alimentação da placa eletrônica.                     |
+----------------------+------------------------------------------------------+
| Integração V1        | Desenvolver a arquitetura inicial baseada em         |
|                      | máquina de estados utilizando dados simulados.       |
+----------------------+------------------------------------------------------+

Tabela 1 – Resumo dos subprojetos desenvolvidos na Etapa 3.

Embora todos esses módulos façam parte do mesmo sistema, eles ainda não compartilham informações entre si nesta etapa. Cada subprojeto possui implementação própria e foi desenvolvido de forma independente, permitindo validar sua funcionalidade antes da integração completa do firmware.

Nas seções seguintes é apresentada uma breve descrição de cada módulo desenvolvido. Os detalhes de implementação, funcionamento e resultados obtidos encontram-se em suas respectivas documentações.

Detecção de Apogeu
==================

Durante esta etapa, o foco concentrou-se exclusivamente na implementação e validação do algoritmo, sem integração com os demais módulos do projeto. Dessa forma, sua avaliação ocorreu de maneira independente, permitindo analisar o comportamento da lógica de detecção antes da integração completa do firmware.

A documentação específica apresenta a fundamentação teórica, o funcionamento do algoritmo, o fluxo de processamento e os critérios utilizados para identificação do apogeu.

`Detecção de Apogeu <deteccao_apogeu>`_

Filtro dos Barômetros
=====================

Assim como os demais módulos desta etapa, o algoritmo foi desenvolvido e validado de forma independente, tendo como objetivo avaliar sua capacidade de produzir uma estimativa mais estável da pressão utilizada posteriormente para o cálculo da altitude.

Os detalhes do algoritmo de filtragem, bem como sua implementação e resultados experimentais, encontram-se na documentação específica.

`Filtro dos Barômetros <filtro_barometros>`_

Projeto da PCB
==============

Além do desenvolvimento dos algoritmos de software, esta etapa contemplou a revisão do projeto eletrônico da placa de circuito impresso, buscando identificar melhorias antes da fabricação da primeira versão do hardware.

As atividades realizadas incluíram a revisão do esquemático elétrico, a verificação dos footprints utilizados, ajustes no posicionamento dos componentes e adequações no circuito de alimentação da placa.

Durante essa revisão foi identificado que o servomotor SG90 não poderia ser alimentado diretamente pela bateria LiPo 2S utilizada no projeto. Embora o servo suporte tensões de até aproximadamente 6 V, a bateria pode atingir cerca de 8,4 V quando totalmente carregada. Dessa forma, foi necessária a inclusão de um conversor de tensão dedicado para garantir que o componente opere dentro das especificações do fabricante e aumentar a confiabilidade do sistema.

A documentação específica apresenta todas as modificações realizadas, bem como os arquivos do projeto desenvolvidos no KiCad.

`Projeto da PCB <PCB>`_

Integração V1
=============

Nesta etapa, a integração teve como finalidade verificar o funcionamento da máquina de estados, a organização das tarefas e a comunicação entre os módulos presentes nesse ambiente de testes. Como os algoritmos de detecção de apogeu e filtragem dos barômetros ainda estavam sendo desenvolvidos de forma independente, foram utilizados dados simulados representando um perfil típico de voo para validar o comportamento da aplicação.

Essa abordagem permitiu avaliar a estrutura inicial do firmware antes da integração dos algoritmos desenvolvidos nos demais subprojetos, atividade prevista para a etapa seguinte.

A documentação específica descreve a arquitetura implementada, a organização das tarefas e o funcionamento da máquina de estados utilizada durante os testes.

`Integração V1 <Integracao_V1>`_

Referências
***********

[1] `Tutorial I2C ESP32 <https://microcontrollerslab.com/esp32-i2c-communication-tutorial-arduino-ide>`_

[2] `Tutorial Itemis Create <https://www.itemis.com/en/products/itemis-create/documentation/tutorials>`_

[3] `Documentação ESP-IDF <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_

[4] `Documentação LEDC (ESP-IDF) <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html>`_

[5] `Documentação I2C (ESP-IDF) <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_

[6] `Exemplos ESP-IDF <https://github.com/espressif/esp-idf/tree/master/examples>`_

[7] `ESP32 as I2C Master <https://www.youtube.com/watch?v=Snp6iTu1R7E>`_
