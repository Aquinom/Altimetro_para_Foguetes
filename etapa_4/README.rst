Etapa 4
#######

.. contents::
   :local:
   :depth: 2


Visão Geral
***********

A Etapa 4 teve como objetivo realizar a validação experimental do altímetro desenvolvido ao longo do projeto, por meio de testes em campo e da consolidação da versão final do hardware.

Os objetivos previstos para esta etapa foram:

* testes e validações em campo da detecção de queda utilizando o acelerômetro;
* testes e validações em campo da estimativa de altitude utilizando os barômetros;
* testes e validações em campo do acionamento do sistema de recuperação por meio do servomotor;
* testes e validações em campo da sinalização sonora utilizando o buzzer após a aterrissagem;
* finalização do esquemático elétrico e do layout da PCB;
* montagem e validação da placa de circuito impresso.

Durante o desenvolvimento da etapa, o firmware integrado foi concluído e a primeira versão da PCB foi fabricada e montada. Entretanto, durante os testes iniciais foi identificado um defeito na placa, impossibilitando a realização dos ensaios em campo previstos para validação completa do sistema.

Desenvolvimento
***************

Nesta etapa foi concluída a integração do firmware desenvolvido ao longo do projeto, reunindo os módulos responsáveis pela aquisição dos sensores, processamento dos dados, detecção de apogeu e gerenciamento da máquina de estados em uma única aplicação embarcada.

Paralelamente ao desenvolvimento do software, foi finalizado o projeto eletrônico da placa de circuito impresso, incluindo o esquemático definitivo, o layout e a montagem da primeira versão da PCB.

As seções a seguir apresentam uma visão geral dos principais resultados obtidos nesta etapa. Os detalhes de implementação encontram-se nas documentações específicas.

Código Final
============

O firmware desenvolvido nesta etapa representa a versão integrada do sistema embarcado do altímetro.

Sua documentação apresenta a arquitetura completa do software, a organização dos módulos, a estrutura do projeto e o funcionamento dos principais componentes responsáveis pela aquisição dos sensores, processamento das informações e gerenciamento da aplicação.

Os detalhes completos do firmware podem ser consultados na documentação específica.

`Código Final <cod_final>`_

PCB
===

Nesta etapa também foi concluído o desenvolvimento da versão final da placa de circuito impresso do altímetro.

A documentação correspondente apresenta o esquemático final, o layout da PCB, imagens da placa montada e as principais decisões adotadas durante o desenvolvimento do hardware.

Os detalhes do projeto eletrônico encontram-se na documentação específica.

`Projeto da PCB <PCB>`_

Testes e Validações
==================

Os testes previstos para esta etapa incluíam a validação em campo do funcionamento integrado do sistema, contemplando:

* detecção da queda por meio do acelerômetro;
* estimativa de altitude utilizando os três barômetros;
* acionamento do sistema de recuperação por meio do servomotor;
* sinalização sonora utilizando o buzzer após a aterrissagem.

Antes da montagem da placa de circuito impresso, foram realizados testes em bancada utilizando uma montagem em protoboard contendo todos os componentes do sistema. Esses ensaios demonstraram o funcionamento integrado do firmware e validaram a comunicação entre os sensores, atuadores e os algoritmos desenvolvidos ao longo do projeto.

Entretanto, durante a montagem e os testes iniciais da primeira versão da PCB foi identificado um defeito de hardware que comprometeu o funcionamento da placa. Em razão desse problema, Foi utilizado uma placa de fenolite perfurada, para tentar efetuar as correções necessárias, e assim, fazer a validação de campo necessária.

Para a placa em questão algumas questões foram alteradas. O Buzzer e os sensores passaram a ser alimentados pelo próprio ESP, enquanto o servo e o ESP foram alimentados diretamente por uma bateria que passava por um regulador de tensão de 5V.

Após efetuadas as devidas mudanças, o teste de validação de campo foi efetuado, e obteve o resultado esperado.

Para mais detalhes sobre os problemas encontrados durante a fabricação da PCB favor referir a `PCB <PCB>`_.

Considerações Finais
********************

A Etapa 4 consolidou o desenvolvimento do firmware e do hardware do altímetro, resultando na integração completa dos módulos de software e na fabricação da primeira versão da PCB.

Os testes em bancada somados com as validações de campo provaram a efetividade do nosso projeto, que foi capaz de realizar todas as funções que eram requisitos de projeto, como detectar pico, abrir o paraquedas, salvar a altitude máxima em memória não-volátil e beeps continuos do buzzer no seu estágio final.

Referências
***********

[1] `Documentação ESP-IDF <https://docs.espressif.com/projects/esp-idf/en/latest/esp32/>`_

[2] `Documentação FreeRTOS <https://www.freertos.org/>`_

[3] `Documentação Itemis Create <https://www.itemis.com/en/products/itemis-create/documentation>`_

[4] `Datasheet BMP280 <https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf>`_

[5] `Datasheet MPU6050 <https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/RM-MPU-6000A.pdf>`_

[6] `ESP32 Technical Reference Manual <https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf>`_
