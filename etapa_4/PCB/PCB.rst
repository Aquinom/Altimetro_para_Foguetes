Layout Final e Fabricação da PCB
***********

Após a conclusão do esquemático elétrico, foram realizados os ajustes finais no layout da placa de circuito impresso utilizando o software KiCad, versão 10.0.1. A PCB foi projetada com dimensões de 90 mm × 60 mm e estrutura de dupla face.

Durante essa etapa, a largura das trilhas foi aumentada de 30 mils para 60 mils. Essa alteração teve como principal objetivo facilitar a fabricação pelo método de transferência térmica, reduzir a possibilidade de rompimento das trilhas durante a corrosão e proporcionar maior tolerância às limitações do processo artesanal.

Também foram modificados os pinos utilizados pelos barramentos I2C do ESP32. O primeiro barramento I2C foi configurado nos pinos GPIO18 e GPIO19, enquanto o segundo barramento foi configurado nos pinos GPIO21 e GPIO22. Essa alteração permitiu uma melhor distribuição física dos sensores e das conexões no layout da placa.

A face inferior da PCB foi utilizada para as trilhas de sinais e alimentação. A face superior não recebeu um layout de trilhas convencional, sendo mantida como um plano de terra contínuo. Nos pontos em que foi necessário conectar o terra das duas faces, foram confeccionadas vias manualmente utilizando pequenos segmentos de fio de cobre soldados nos dois lados da placa.

Furação e corte da placa
***********

A furação e o corte do contorno da PCB foram realizados utilizando uma CNC Genmitsu 3018 Pro. Todos os furos destinados aos terminais dos componentes foram produzidos com diâmetro de 1 mm, utilizando uma ferramenta com revestimento de AlTiN. O corte externo da placa foi realizado utilizando uma fresa de 3 mm, também revestida com AlTiN.

A sequência de fabricação adotada foi:

#. realização dos furos passantes;
#. corte do contorno externo da PCB;
#. alinhamento do desenho da face inferior com os furos;
#. transferência térmica do desenho;
#. corrosão do cobre;
#. limpeza, inspeção e correção das trilhas;
#. soldagem dos componentes e das vias de cobre.

Como os furos foram produzidos antes da transferência térmica, eles foram utilizados como referência para o alinhamento do desenho da face inferior. Mesmo com esse procedimento, foi observado um erro de alinhamento de aproximadamente 0,5 mm nos eixos X e Y. Apesar disso, os furos foram produzidos corretamente e permaneceram utilizáveis.

Tentativa de fabricação por fresagem
***********

Antes da utilização do método de transferência térmica, foi realizada uma tentativa de produzir diretamente as trilhas por fresagem. Para isso, foi utilizada uma fresa do tipo V-bit com ponta de 0,1 mm e ângulo de 30 graus.

Entretanto, a tentativa não apresentou resultados satisfatórios. A CNC utilizada não possuía um sistema de nivelamento automático da superfície da placa. Como consequência, em algumas regiões a ferramenta não alcançou o cobre, enquanto em outras penetrou excessivamente no material da PCB.

Essa variação de profundidade impossibilitou a obtenção de trilhas uniformes. Por esse motivo, a fabricação por fresagem foi interrompida e substituída pelo método de transferência térmica seguido de corrosão química.

Transferência térmica
***********

O desenho da face inferior foi impresso utilizando uma impressora a laser e papel transfer de 100 g/m² da marca Adespan. O arquivo foi utilizado sem a aplicação de espelhamento adicional.

A transferência do toner para a placa foi realizada utilizando um ferro de passar roupa convencional, configurado na temperatura correspondente a tecidos de algodão. Como a face superior foi mantida como um plano de terra contínuo, não foi necessário realizar a transferência de um desenho nessa face.

Após o posicionamento e o alinhamento do papel com os furos previamente produzidos, foi aplicada pressão e calor sobre toda a superfície. Depois do resfriamento, o papel foi removido e a aderência do toner ao cobre foi inspecionada visualmente.

Corrosão da PCB
***********

A remoção do cobre da face inferior foi realizada utilizando uma solução de percloreto férrico previamente utilizada. O processo ocorreu em temperatura ambiente e teve duração aproximada de uma hora e trinta minutos.

Durante a corrosão, a placa foi movimentada esporadicamente para renovar o contato da solução com a superfície de cobre. A PCB foi retirada antes da remoção completa de algumas pequenas regiões de cobre. Essa decisão foi tomada para reduzir o risco de corrosão excessiva e evitar que as trilhas já formadas fossem danificadas.

Após a corrosão, as regiões em que ainda havia cobre indesejado foram inspecionadas e corrigidas manualmente.

Problemas de Fabricação
***********

Foram necessárias seis tentativas de transferência térmica e corrosão até que fosse obtido um resultado considerado utilizável. Nas cinco primeiras tentativas, o principal problema encontrado foi a aderência incompleta do toner ao cobre.

Embora o desenho transferido apresentasse boa nitidez, pequenas regiões não aderiam corretamente à placa. Essas falhas deixavam partes do cobre expostas ao percloreto férrico, provocando interrupções nas trilhas durante a corrosão.

Na melhor tentativa, as falhas identificadas antes da corrosão foram preenchidas utilizando uma caneta permanente para CD. A tinta da caneta funcionou como uma proteção adicional, impedindo que o percloreto férrico entrasse em contato com o cobre nas regiões corrigidas.

Algumas trilhas ainda apresentaram interrupções após a corrosão. Além disso, nas regiões dos circuitos integrados, o pequeno espaçamento entre os terminais fez com que permanecessem ligações indesejadas de cobre. Essas regiões foram isoladas manualmente após a corrosão para evitar curtos entre os pinos.

Falha no acelerômetro
***********

Apesar das correções realizadas, a melhor placa produzida apresentou uma falha grave durante os testes. Um dos terminais de alimentação do acelerômetro entrou em contato com o cobre da face superior.

Como a face superior havia sido mantida como um plano de terra contínuo, deveria ter sido criada uma região sem cobre ao redor dos furos correspondentes aos terminais que não eram conectados ao terra. Essa área de isolamento, também denominada *clearance*, não foi prevista no layout.

O curto-circuito foi identificado por meio do teste de continuidade com multímetro e pelo acionamento da limitação de corrente da fonte de bancada. A falha também resultou na queima do componente conectado à região.

Esse problema demonstrou que, mesmo quando uma das faces é utilizada principalmente como plano de terra, é necessário prever áreas de isolamento ao redor de todos os furos e terminais que conduzem sinais ou tensões diferentes do potencial de terra.

Após os problemas encontrados na produção artesanal da placa de duas faces, decidiu-se interromper essa abordagem e realizar a montagem do circuito utilizando uma placa perfurada.


Referências
***********
(colocar referencias melhores)
[1] `Datasheet MPU6050 <https://cdn.sparkfun.com/datasheets/Sensors/Accelerometers/RM-MPU-6000A.pdf>`_

[2] `Datasheet SG90 <https://www.friendlywire.com/projects/ne555-servo-safe/SG90-datasheet.pdf>`_

[3] `Datasheet BMP280 <https://cdn-shop.adafruit.com/datasheets/BST-BMP280-DS001-11.pdf>`_

[4] `Datasheet ESP32 <https://documentation.espressif.com/esp32_datasheet_en.pdf>`_


