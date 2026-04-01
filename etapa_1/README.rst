Etapa 1
#######

.. contents::
   :local:
   :depth: 2


Visão geral
***********

A Etapa 1 tem como objetivo estruturar os requisitos do projeto, de modo a definir a escolha dos componentes que atendam às necessidades do projeto.

A partir dessa definição, são realizadas a seleção e aquisição dos componentes, bem como a pesquisa das bibliotecas necessárias para o desenvolvimento. Além disso, esta etapa contempla o estudo dos sensores envolvidos e a elaboração de um diagrama de blocos simples, servindo como base para as próximas fases do projeto.

Desenvolvimento
***************

O dispositivo sendo desenvolvido deve ser capaz de medir com confiança e precisão altidude do foguete, ser capaz de abrir um sistema de paraquedas e emitir um sinal sonoro para facilitar sua localização no solo após aterrisagem. Para garantir os requisitos de altitude, serão integrados múltiplos módulos com barômetros, o qual passará por um filtro digital de modo que a medida seja a mais correta possível. Será implementado um atuador mecânico, neste caso um servo motor, para abertura do paraquedas quando for detectado o momento certo. Os dados de um acelerômetro serão cruzados com o barômetro para detectar apogeu. Após a aterrisagem do foguete, um buzzer será acionado de forma intermitente oque facilitará encontrar o foguete. Para alimentar todos os sitemas, uma bateria leve será embarcada.

Para auxiliar na seleção dos componentes de cada requisito, foi realizado uma pesquisa de alguns sensores e criado uma tabela para cada um que agrega os critérios de seleção mais importantes:




Testes
======

Descrição dos testes/validações realizadas.


Diagrama de blocos
================================
![Diagrama_de_blocos_v2](https://github.com/Aquinom/Altimetro_para_Foguetes/blob/main/etapa_1/imagens/Diagrama%20de%20blocos-v2.png)


Referências (links/datasheets/livros)
*************************************

- `nRF Connect SDK <https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.4.2/nrf/getting_started/modifying.html#configure-application>`_


