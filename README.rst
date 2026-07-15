Projeto de Altímetro para Foguetes
##################################

.. contents::
   :local:
   :depth: 2

Visão Geral
***********

Este repositório reúne o desenvolvimento de um altímetro embarcado para foguetes de pequeno porte, desenvolvido no âmbito da disciplina **Projeto Integrador III**, do curso de **Engenharia Eletrônica** do **Instituto Federal de Santa Catarina (IFSC)**.

A disciplina é ministrada pelos professores **Renan Augusto Starke** e **Matheus Leitzke Pinto**, responsáveis pela orientação e acompanhamento do projeto ao longo de seu desenvolvimento.

O projeto foi desenvolvido pelos acadêmicos:

* Caio Neves Meira;
* Marcelo Zampieri Pereira da Silva;
* Rafael Aquino de Meireles.

O objetivo do projeto é desenvolver um sistema embarcado capaz de monitorar o voo do foguete, estimar sua altitude em tempo real, detectar automaticamente o apogeu e controlar os dispositivos responsáveis pela abertura do paraquedas e pela sinalização sonora após o pouso, contribuindo para a recuperação do foguete.

O firmware foi desenvolvido utilizando o microcontrolador ESP32, o framework ESP-IDF e o sistema operacional FreeRTOS, permitindo a execução concorrente das tarefas de aquisição dos sensores, processamento dos dados e controle da missão.

Durante o desenvolvimento, o projeto foi dividido em quatro etapas incrementais, possibilitando a validação individual dos módulos de hardware e software antes da integração completa do sistema.

Objetivos do Projeto
********************

O sistema desenvolvido possui como principais funcionalidades:

* estimativa da altitude utilizando sensores barométricos;
* detecção automática do apogeu;
* acionamento automático do servo motor para abertura do paraquedas;
* acionamento do buzzer após o pouso;
* armazenamento da maior altitude atingida durante o voo;
* arquitetura modular baseada em máquina de estados.

Hardware Utilizado
******************

Os principais componentes utilizados durante o desenvolvimento são:

* 1xESP32 DevKit;
* 3x sensores barométricos BMP280;
* 1xacelerômetro MPU6050;
* 1x servo motor SG90;
* 1x buzzer piezoelétrico;
* 1x bateria Li-Po 2s;

Como Utilizar
*************

O firmware foi desenvolvido utilizando o framework ESP-IDF.

1. Clone este repositório.

::

    git clone https://github.com/Aquinom/Altimetro_para_Foguetes.git

2. Instale o ESP-IDF conforme a documentação oficial da Espressif.

3. Abra o projeto utilizando o Visual Studio Code com a extensão ESP-IDF ou outro ambiente compatível.

4. Utilize a versão final do firmware disponível em etapa_4/cod_final: `Código final <etapa_4/cod_final>`_

5. Compile o projeto.

::

    idf.py build

6. Grave o firmware no ESP32.

::

    idf.py flash

7. Abra o monitor serial para acompanhar a execução do firmware.

::

    idf.py monitor

Durante a execução serão exibidas informações referentes às leituras dos sensores, altitude estimada, velocidade vertical, estado atual da máquina de estados e demais informações de telemetria.

Montagem do Hardware
********************

A montagem do circuito deve ser realizada utilizando a placa desenvolvida para a versão final do projeto.

Os arquivos referentes ao esquemático, layout e documentação da PCB encontram-se em: `PCB final <etapa_4/PCB>`_

Durante os testes, o circuito deve ser alimentado utilizando uma bateria de pelo menos 6,2V.

Embora o ESP32 possa ser alimentado pela porta USB durante a gravação do firmware, essa alimentação não fornece corrente suficiente para o correto acionamento do servo motor. Dessa forma, recomenda-se utilizar a bateria durante todos os testes que envolvam a abertura do compartimento do paraquedas.

Organização do Repositório
**************************

O repositório está organizado conforme apresentado abaixo.

::

    ├── etapa_1/
    ├── etapa_2/
    ├── etapa_3/
    ├── etapa_4/
    └── README.rst

Cada diretório contém a documentação correspondente à etapa de desenvolvimento, incluindo códigos-fonte, diagramas, esquemáticos, arquivos da PCB e documentação técnica.

Etapas do Desenvolvimento
*************************

O desenvolvimento do altímetro foi dividido em quatro etapas incrementais, permitindo validar individualmente cada módulo antes da integração completa do sistema.

Etapa 1 — Definição do Projeto
==============================

Nesta etapa foram realizadas as atividades iniciais de definição do projeto, compreendendo:

* definição dos requisitos do projeto;
* definição e aquisição dos componentes;
* pesquisa das bibliotecas necessárias ao desenvolvimento;
* estudo dos sensores utilizados;
* elaboração do diagrama simplificado de hardware.

Para mais detalhes, acesse: `Etapa 1 - Definição do projeto <etapa_1>`_

Etapa 2 — Desenvolvimento dos Periféricos
=========================================

Nesta etapa foram implementados e validados os principais periféricos utilizados pelo sistema:

* comunicação com o barômetro utilizando o ESP32;
* comunicação com o acelerômetro;
* acionamento do buzzer;
* acionamento do servo motor;
* desenvolvimento do esquemático preliminar;
* modelagem inicial da máquina de estados.

Para mais detalhes, acesse: `Etapa 2 - Desenvolvimento dos periféricos <etapa_2>`_

Etapa 3 — Integração do Firmware
================================

Nesta etapa foi realizada a integração entre hardware e software, contemplando:

* estimativa inicial de altura com filtro simples;
* implementação inicial do algoritmo de detecção de subida e queda;
* revisão do esquemático eletrônico;
* desenvolvimento do layout preliminar da PCB;
* ajustes de hardware e integração do firmware.

Para mais detalhes, acesse: `Etapa 3 - Integração do firmware <etapa_3>`_

Etapa 4 — Validação Final
=========================

A etapa final contempla a integração completa do sistema e sua validação em condições reais de operação, incluindo:

* testes em campo da detecção de queda utilizando o acelerômetro;
* testes em campo da estimativa de altitude utilizando os barômetros;
* validação do acionamento do servo motor para abertura do paraquedas;
* validação do acionamento do buzzer após o pouso;
* desenvolvimento do esquemático final;
* desenvolvimento do layout final da PCB;
* montagem e validação da placa de circuito impresso.

Para mais detalhes, acesse: `Etapa 4 - Validação final <etapa_4>`_

Arquitetura Geral do Firmware
*****************************

O firmware foi desenvolvido utilizando o sistema operacional FreeRTOS, sendo dividido em tarefas independentes responsáveis pela aquisição dos sensores, processamento das informações, execução da máquina de estados e comunicação entre os módulos do sistema.

De forma simplificada, o funcionamento do firmware pode ser representado pelo fluxo abaixo.

::

                Sensores
          (BMP280 e MPU6050)
                     │
                     ▼
          Aquisição dos Dados
                     │
                     ▼
         Filtragem das Leituras
                     │
                     ▼
      Estimativa de Altitude e
        Velocidade Vertical
                     │
                     ▼
          Máquina de Estados
                     │
                     ▼
          Detecção do Apogeu
                     │
                     ▼
       Acionamento do Servo
                     │
                     ▼
       Acionamento do Buzzer
                     │
                     ▼
       Armazenamento dos Dados

A arquitetura modular adotada reduz o acoplamento entre os módulos do firmware, facilitando futuras expansões, manutenção do código e validação individual de cada componente.

