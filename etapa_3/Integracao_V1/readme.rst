Integração da Máquina de Estados (Versão 1)
##########################################

.. contents::
   :local:
   :depth: 2

Objetivos da Etapa
******************

Esta etapa teve como objetivo estabelecer a arquitetura básica do firmware do altímetro, integrando a máquina de estados ao restante da aplicação embarcada. Para isso, foram desenvolvidas as seguintes atividades:

* modelagem da máquina de estados utilizando o Itemis CREATE;
* integração do código gerado ao firmware do ESP32;
* definição da estrutura de execução baseada em tarefas do FreeRTOS;
* organização da comunicação entre os módulos do sistema;
* estudo dos mecanismos de sincronização para acesso aos recursos compartilhados.

Essas atividades servem como base para a integração dos algoritmos de processamento dos sensores e detecção de eventos desenvolvidos nas etapas seguintes.

Visão Geral
***********

O firmware do altímetro é organizado em torno de uma máquina de estados responsável por controlar o comportamento do sistema durante as diferentes fases do voo do foguete. Essa abordagem permite separar claramente as responsabilidades de cada etapa da missão, tornando a lógica de controle mais organizada, previsível e de fácil manutenção.

Nesta primeira versão da integração foi desenvolvida a estrutura responsável por conectar a máquina de estados ao restante do firmware embarcado, permitindo que as informações provenientes dos sensores sejam utilizadas para controlar as transições entre os estados do sistema.

Para o desenvolvimento da máquina de estados foi utilizado o **Itemis CREATE**, ferramenta que permite modelar sistemas reativos por meio de diagramas de estados e gerar automaticamente a implementação correspondente em linguagem C. Dessa forma, a lógica de controle permanece concentrada na máquina de estados, enquanto os demais módulos do firmware ficam responsáveis pela aquisição dos sensores, processamento dos dados e comunicação entre as tarefas.

Arquitetura Geral
***********

O funcionamento do firmware pode ser resumido pelo fluxo apresentado abaixo.

::

                    Sensores
            (BMP280 e Acelerômetro)
                       │
                       ▼
              Processamento dos dados
      (Filtragem, cálculo de altitude e velocidade)
                       │
                       ▼
               Máquina de Estados
                       │
                       ▼
            Decisões referentes ao voo
                       │
                       ▼
             Interface de comunicação

A separação entre aquisição dos sensores, processamento e tomada de decisão reduz o acoplamento entre os módulos do sistema, facilitando futuras alterações e a validação individual de cada componente.

Máquina de Estados
***********

A máquina de estados representa as diferentes fases de operação do altímetro ao longo do voo do foguete. Cada estado descreve um comportamento específico do sistema, enquanto as transições ocorrem quando determinados eventos são detectados.

Nesta versão do projeto, a máquina foi estruturada para representar as principais fases da missão do foguete, desde a inicialização do sistema até o término do voo.

* Inicialização do sistema;
* Espera pelo lançamento;
* Subida do foguete;
* Detecção do apogeu;
* Descida;
* Finalização da missão.

Cada mudança de estado depende das informações fornecidas pelos módulos responsáveis pelo processamento dos sensores, como altitude estimada, velocidade vertical e aceleração.

Dessa forma, toda a lógica de decisão permanece centralizada na máquina de estados, evitando que decisões de controle fiquem distribuídas entre diferentes partes do firmware.

Podemos ver a maquina proposta na **Figura 1** e suas devidas mudanças de estado.

.. image:: /etapa_2/Imagens/Statechart.jpeg
   :alt: Diagrama da máquina de estados do firmware
   :align: center
   :width: 600px

**Figura 1.** Máquina de estados utilizada pelo firmware.

Utilização do Itemis CREATE
***********

O Itemis CREATE foi adotado para modelar graficamente a máquina de estados e gerar automaticamente sua implementação em linguagem C.

A ferramenta é responsável pela geração da estrutura completa da máquina, incluindo:

* definição dos estados;
* eventos;
* transições;
* variáveis internas;
* funções de execução.

O firmware desenvolvido para o ESP32 incorpora esse código gerado automaticamente e realiza apenas a integração entre a máquina de estados e os módulos responsáveis pela aquisição e processamento dos sensores.

Essa abordagem reduz a possibilidade de inconsistências durante a implementação da lógica de controle e facilita futuras modificações no comportamento do sistema.

Organização das Tasks
*********************

O firmware utiliza o FreeRTOS para executar múltiplas tarefas de forma concorrente. Cada tarefa possui uma responsabilidade específica, permitindo que a aquisição dos sensores mantenha sua frequência de atualização enquanto outras atividades são executadas em paralelo.

**Sensor Task**
   Responsável pela aquisição periódica dos sensores embarcados.

**Processing Task**
   Responsável pelo processamento das leituras dos sensores e disponibilização das informações para os demais módulos.

**State Machine Task**
   Responsável pela execução da máquina de estados do altímetro e pelo gerenciamento das transições entre os estados do voo.

**Communication Task**
   Responsável pelo envio das informações pela interface serial.

Essa divisão permite que a aquisição dos sensores mantenha uma frequência constante, enquanto outras tarefas executam processamento ou comunicação sem interferir diretamente no funcionamento do sistema.

Integração entre as Tasks
***********

Embora cada tarefa execute uma função específica, todas compartilham informações necessárias para o funcionamento do firmware.

Os módulos responsáveis pela leitura dos sensores disponibilizam seus resultados para a máquina de estados, que utiliza essas informações para determinar o comportamento do sistema durante cada fase do voo.

De forma semelhante, as informações produzidas pela máquina de estados podem ser utilizadas por outras tarefas responsáveis pelo armazenamento ou transmissão dos dados.

Essa organização reduz o acoplamento entre os módulos e facilita a expansão do projeto nas próximas etapas.

Sincronização de Dados
***********

Como diferentes tarefas podem acessar simultaneamente estruturas compartilhadas do firmware, torna-se necessário controlar esse acesso para evitar inconsistências.

Para isso, foi estudada a utilização de mecanismos de sincronização disponibilizados pelo FreeRTOS, como os mutexes.

O uso de mutex garante acesso exclusivo aos recursos compartilhados durante operações críticas, evitando condições de corrida e assegurando que diferentes tarefas não modifiquem simultaneamente os mesmos dados.

Embora a implementação completa desse mecanismo ocorra nas etapas seguintes do projeto, sua utilização já foi considerada durante a definição da arquitetura do firmware.

Resultados da Integração
***********

Ao final desta etapa foi estabelecida a arquitetura básica responsável pela integração entre a máquina de estados e o restante do firmware.

Os principais resultados obtidos foram:

* estruturação da máquina de estados utilizando o Itemis CREATE;
* integração do código gerado ao firmware do ESP32;
* organização das tarefas utilizando o FreeRTOS;
* definição da comunicação entre os módulos do sistema;
* preparação da arquitetura para a integração dos algoritmos de processamento e detecção de eventos nas próximas etapas.

Estado Atual da Implementação
*****************************

Ao término desta etapa encontram-se implementados:

* a máquina de estados desenvolvida no Itemis CREATE;
* a integração do código gerado ao firmware do ESP32;
* a organização inicial das tarefas utilizando o FreeRTOS;
* a definição da comunicação entre os principais módulos do sistema.

Nas próximas etapas serão incorporados os algoritmos responsáveis pela filtragem dos sensores, estimação de altitude e velocidade, detecção de apogeu e acionamento dos dispositivos do sistema.

Referências
***********

[1] `Tutorial I2C ESP32 <https://microcontrollerslab.com/esp32-i2c-communication-tutorial-arduino-ide>`_

[2] `Tutorial Itemis Create <https://www.itemis.com/en/products/itemis-create/documentation/tutorials>`_

[3] `Documentação Espressif <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_

[4] `Documentação LEDC (ESP-IDF) <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html>`_

[5] `Documentação I2C (ESP-IDF) <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_

[6] `Exemplos ESP-IDF Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_

[7] `ESP32 as I2C Master <https://www.youtube.com/watch?v=Snp6iTu1R7E>`_
