# Máquina de Estados e Gerenciamento de Tasks

Este projeto utiliza o **Itemis Create** para desenvolver a máquina de estados do altímetro de foguete.
A ferramenta permite modelar visualmente os estados e transições do sistema, facilitando a organização da lógica do firmware e reduzindo erros durante o desenvolvimento.

Além da modelagem visual, o Itemis também gera automaticamente parte do código em linguagem C, tornando a integração com o ESP32 mais simples e padronizada.

---

# FreeRTOS

O projeto utiliza **FreeRTOS** para gerenciamento das tasks executadas pelo microcontrolador.

A utilização de múltiplas tasks permite que diferentes partes do sistema funcionem de forma concorrente e independente, deixando o firmware mais modular e organizado.

Atualmente o sistema possui tasks para:

* Execução da máquina de estados
* Simulação do acelerômetro
* Simulação do barômetro
* Detecção de pico

---

# Simulação dos Sensores

Nesta etapa do desenvolvimento, os sensores ainda estão sendo simulados utilizando valores aleatórios.

A implementação física do acelerômetro e do barômetro será realizada posteriormente, sendo integrada ao restante da aplicação já estruturada.

---

# Uso de Mutex

Também foi estudado o uso de **mutex** para controle de acesso a recursos compartilhados entre múltiplas tasks.

A intenção é utilizar mutex futuramente para evitar conflitos de acesso simultâneo às variáveis globais do sistema, aumentando a estabilidade e confiabilidade da aplicação embarcada.
