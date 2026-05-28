Etapa 3
#######

.. contents::
   :local:
   :depth: 2


Visão geral
***********

Para a etapa 3 o desenvolvimento se resume em 4 sub-entregas:

* Estimativa inicial de altura com filtro simples
* Implementação inicial do algoritmo de detecção de subida
* Revisão do esquemático e layout preliminar
* Ajustes de hardware e integração

O primeiro item se resume em criar o algoritmo de estimativa de altura com filtro e verificar se a medida é coerente. 


Desenvolvimento
***************

Para fazer uma estimativa inicial de altura é necessário implementar um algoritmo que pega o valor inicial de pressão dos três barômetros e utiliza como zero, e em seguida pegar o valor de pressão no apogeu, faz a subtração dos valores e converte para metros para obter a altitude acima do nível do solo. Podemos melhorar essa leitura implementado um simples filtro e retirando barômetros que deem valores muitos dicrepantes entre si. Um exemplo de implementação é o filtro média móvel e atribuição de pesos para cada barômetro. Para ter certeza que a medida obtida é coerente, primeiro devemos comparar com um valor de altitude conhecido ou com outro sensor que seja confiável. Devemos tomar muito cuidado na implementação do filtro para que ele não seja lento demais ou que seja ruidoso demais.

Também percebemos a necessidade da implementação de outro algoritmo para detectar a subida do foguete e detecção de apogeu. Isso será feito utilizando os dados do acelerômetro e do barômetro e criando um algoritmo que cruza ambos os dados. O lançamento de um foguete possui uma curva típica de Aceleração x Tempo conforme podemos ver abaixo:

Criar um algoritmo de detecção de lançamento pode levar em contas estas mesmas variáveis.

A primeira versão do equemático e da PCB passarão por uma revisão minuciosa de forma a identificar problemas e corrigi-los antes de produzir a PCB. Nesta etapa ainda é posssível fazer modificações de hardware como implementação de mais circuitos e verificar se as escolhas de sensores e atuadores foram adequadas. Foi observado que há a necessidade de um conversor para o servo motor pois o servo SG90 utilizado é tolerante até 6V. A tensão de operação da bateria 2S vai ficar entre 6,6V e 8,4V; desse modo fica evidente a necessidade desta modificação. Outro ponto de implementação é um footprint modificado para os módulos.


Detecção de apogeu
======

Segue abaixo o link para acesso ao código de detecção de apogeu desenvolvido durante esta etapa

`Detecção de apogeu <deteccao_apogeu>`_

Filtro barômetros
======

Segue abaixo o link para acesso ao código do filtro dos barômetros desenvolvido durante esta etapa

`Filtro barometros <filtro_barometros>`_

Esquemático e Layout da Placa
======

Abaixo temos o link que leva para o projeto KiCAD, esquemático em PDF e imagens do layout final da PCB.

`Projeto da PCB <PCB>`_

Integração V1
======

Abaixo temos o link que leva para a parte de projeto onde foram executados os testes de integração. Foi utilizado em conjunto com a máquina de estados gerada pelo Itemis Create, o FreeRtos como sistema operacional de tempo real para manipulação e gerenciamento das tasks do nosso sistema.

No momento, os sensores do sistema foram simulados gerando valores aleatórios.

Os códigos para replicação dos testes também estão disponíveis no link abaixo.

`IntegracaoV1 <Integracao_V1>`_

Referências (links/datasheets/livros)
*************************************

- `Tutorial I2C ESP32 <https://microcontrollerslab.com/esp32-i2c-communication-tutorial-arduino-ide>`_
- `Tutorial Itemis Create <https://www.itemis.com/en/products/itemis-create/documentation/tutorials>`_
- `Documentação Espressif <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_
- `Documentação LED-C utilizado para programar o SG90 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html>`_
- `Documentação I2C utilizado para programar o MPU6050 <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html>`_
- `Exemplos de programação ESP32 Espressif <https://github.com/espressif/esp-idf/tree/master/examples>`_
- `ESP32 as I2C Master <https://www.youtube.com/watch?v=Snp6iTu1R7E>`_


