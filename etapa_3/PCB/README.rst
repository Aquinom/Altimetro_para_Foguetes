Desenvolvimento da PCB
######################

.. contents::
   :local:
   :depth: 2

Objetivos da Etapa
******************

Esta etapa teve como objetivo desenvolver a primeira versão da placa de circuito impresso (PCB) do altímetro de foguete, reunindo em uma única placa todos os componentes eletrônicos necessários para aquisição dos sensores, processamento das informações e acionamento dos dispositivos do sistema.

Além da integração dos componentes, buscou-se organizar o posicionamento dos dispositivos e o roteamento das trilhas, estabelecendo uma arquitetura que facilite futuras modificações e reduza a necessidade de conexões externas.

Visão Geral
***********

Após a definição dos componentes eletrônicos utilizados no projeto, iniciou-se o desenvolvimento da PCB utilizando o software KiCAD 10.

A placa foi projetada para concentrar todos os principais módulos do sistema embarcado, reduzindo a quantidade de cabos entre os componentes e aumentando a confiabilidade das conexões durante o voo do foguete.

Nesta primeira versão, o foco foi validar a disposição dos componentes, a alimentação dos dispositivos e o roteamento dos sinais principais.

Arquitetura da PCB
******************

A Figura 1 apresenta a visão geral da placa desenvolvida.

.. image:: etapa_3/images/PCB_3D.jpg
   :alt: Visualização 3D da PCB.
   :align: center
   :width: 650px

**Figura 1.** Modelo 3D da primeira versão da PCB.

A distribuição dos componentes foi realizada de forma a facilitar a montagem do sistema e minimizar o cruzamento de trilhas, contribuindo para um roteamento mais organizado e para futuras manutenções.

Principais Componentes
**********************

Os principais componentes integrados à PCB são:

* ESP32, responsável pelo processamento do sistema;
* três sensores barométricos BMP280 para estimação da altitude;
* acelerômetro MPU6050 utilizado na estimação da aceleração vertical;
* circuito de alimentação - LiPO 2s;
* conectores para programação e depuração;
* interface para acionamento do servo motor SG90;
* buzzer.

Cada componente foi posicionado considerando tanto a facilidade de montagem quanto a organização das conexões elétricas da placa.

Projeto da Alimentação
**********************

O circuito de alimentação foi desenvolvido para fornecer tensão adequada aos diferentes dispositivos presentes na placa.

Como alguns componentes operam com tensões distintas, foram previstos os circuitos necessários para garantir uma alimentação compatível com cada dispositivo, assegurando o funcionamento adequado do sistema embarcado.

Também foram adicionados capacitores de desacoplamento próximos aos circuitos integrados, reduzindo interferências provenientes das variações de corrente durante a operação.

Roteamento da PCB
*****************

Após o posicionamento dos componentes, foi realizado o roteamento das trilhas elétricas.

Durante essa etapa buscou-se:

* reduzir o comprimento das trilhas críticas;
* minimizar cruzamentos;
* organizar os sinais de alimentação;
* facilitar futuras revisões da placa.

Esses cuidados contribuem para aumentar a confiabilidade elétrica do sistema e simplificar possíveis modificações nas próximas versões da PCB.

Resultados Obtidos
******************

Ao final desta etapa foi desenvolvida a primeira versão funcional da placa de circuito impresso do altímetro.

Os principais resultados obtidos foram:

* integração dos principais componentes eletrônicos em uma única PCB;
* definição da arquitetura elétrica do sistema;
* organização das conexões entre sensores e microcontrolador;
* desenvolvimento do circuito de alimentação;
* preparação da placa para fabricação e validação experimental.

Estado Atual da Implementação
*****************************

Nesta etapa foi concluído o projeto da segunda versão da PCB.

Após sua fabricação e montagem, serão realizados testes elétricos e funcionais para validar o correto funcionamento dos sensores, da alimentação e das interfaces de comunicação, permitindo a integração definitiva ao firmware desenvolvido nas demais etapas do projeto.

Referências
***********

[1] `Datasheet MPU6050 <https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/RM-MPU-6000A.pdf>`_

[2] `Datasheet SG90 <https://www.friendlywire.com/projects/ne555-servo-safe/SG90-datasheet.pdf>`_

[3] `Datasheet BMP280 <https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf>`_

[4] `Datasheet ESP32 <https://documentation.espressif.com/esp32_datasheet_en.pdf>`_

