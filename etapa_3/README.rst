Etapa 3
#######

.. contents::
   :local:
   :depth: 2


Visão geral
***********

Para a etapa 3 o desenvolvimento se resume em 4 sub-entregas:

* Estimativa inicial de altura com filtro simples
* Implementação inicial do algoritmo inicial de detecção de subida
* Revisão do esquemático e layout preliminar
* Ajustes de hardware e integração

O primeiro item se resume em criar o algoritmo de estimativa de altura com filtro e verificar se a medida é coerente. 


Desenvolvimento
***************

Apresentar o desenvolvimento da etapa contendo detalhes de implementação (se houver) de hardware e software. Adicionar pesqusisas realizadas bem como testes realizados.

Para fazer uma estimativa inicial de altura é necessário implementar um algoritmo que pega o valor inicial de pressão dos três barômetros e utiliza como zero, e em seguida pegar o valor de pressão no apogeu, faz a subtração dos valores e converte para metros para obter a altitude acima do nível do solo. Podemos melhorar essa leitura implementado um simples filtro e retirando barômetros que deem valores muitos dicrepantes entre si. Um exemplo de implementação é o filtro média móvel e atribuição de pesos para cada barômetro. Para ter certeza que a medida obtida é coerente, primeiro devemos comparar com um valor de altitude conhecido ou com outro sensor que seja confiável. Devemos tomar muito cuidado na implementação do filtro para que ele não seja lento demais ou que seja ruidoso demais.

Também percebemos a necessidade da implementação de outro algoritmo para detectar a subida do foguete e detecção de apogeu. Isso será feito utilizando os dados do acelerômetro e do barômetro e criando um algoritmo que cruza ambos os dados. O lançamento de um foguete possui uma curva típica de Aceleração x Tempo conforme podemos ver abaixo:

Criar um algoritmo de detecção de lançamento pode levar em contas estas mesmas variáveis.

A primeira versão do equemático e da PCB passarão por uma revisão minuciosa de forma a identificar problemas e corrigi-los antes de produzir a PCB. Nesta etapa ainda é posssível fazer modificações de hardware como implementação de mais circuitos e verificar se as escolhas de sensores e atuadores foram adequadas. Foi observado que há a necessidade de um conversor para o servo motor pois o servo SG90 utilizado é tolerante até 6V. A tensão de operação da bateria 2S vai ficar entre 6,6V e 8,4V; desse modo fica evidente a necessidade desta modificação. Outro ponto de implementação é um footprint modificado para os módulos.


Testes
======

Descrição dos testes/validações realizadas.


(Outras subseções se necessário)
================================


Referências (links/datasheets/livros)
*************************************

- `nRF Connect SDK <https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.4.2/nrf/getting_started/modifying.html#configure-application>`_


