# Breve estudo dos componentes

## Sumário
1. [Buzzer](#buzzer)
2. [Servomotor](#servomotor)
3. [Barômetro](#barometro)
4. [Acelerômetro](#acelerometro)

---

## Buzzer

Componente escolhido: Módulo Buzzer Ativo YL-44

Alimentação: 3,3 ~ 5V

Controle: High / Low

Destacando que, a diferença de um buzzer passivo para um ativo, é que com um buzzer passivo, conseguimos faze-lo emitir diferentes tons variando a frequência, e já o buzzer ativo, só conseguimos faze-lo apitar, ou não apitar.

E como para o projeto em questão a função do Buzzer seria apenas identificar a localidade do foguete após a queda, o ativo já seria suficiente para a função.

Pinos:
- VCC → alimentação  
- GND → terra  
- S → sinal digital  

![Módulo Buzzer 5V](.etapa_1/imagens/buzzer.png)

---

## Servomotor

Componente escolhido: SG90

Micro servo motor que controla a posição angular de 0 ~ 180°.

A função do servomotor será abrir o paraquedas após o apogeu.

Alimentação: 5V

Controle: PWM

Pinos:
- VCC (vermelho) → alimentação  
- GND (marrom) → terra  
- Signal (laranja) → sinal PWM  

Observação: O controle é feito por largura de pulso (PWM), onde diferentes pulsos correspondem a diferentes ângulos.

![SG90](./etapa_1/imagens/sg90.png)

---

## Barômetro

Componente escolhido: BMP280

Sensor de pressão e temperatura.

Terá como função calcular a altitude com base na variação da pressão atmosférica.

Alimentação: 3,3V

Controle: I2C

### Funcionamento físico
O BMP280 utiliza um sensor piezoresistivo. A pressão do ar deforma uma membrana microscópica interna, alterando sua resistência elétrica. Essa variação é convertida em sinal digital. Como a pressão atmosférica diminui com o aumento da altitude, é possível calcular a altura do foguete.

Pinos:
- VCC → alimentação  
- GND → terra  
- SCL → clock I2C  
- SDA → dados I2C  

![BMP280](./etapa_1/imagens/BMP280.png)

---

## Acelerômetro

Componente escolhido: MPU6050

Sensor que combina acelerômetro e giroscópio (6 eixos).

Será utilizado para detectar movimento, aceleração e possível identificação do apogeu do foguete.

Alimentação: 3,3V

Controle: I2C

### Funcionamento físico
O acelerômetro funciona com base em estruturas microscópicas (MEMS) que se deslocam quando submetidas à aceleração. Esse deslocamento altera propriedades elétricas (como capacitância), permitindo medir aceleração nos eixos X, Y e Z.  

Pinos:
- VCC → alimentação  
- GND → terra  
- SCL → clock I2C  
- SDA → dados I2C  
- INT → interrupção (opcional)  

![MPU6050](./etapa_1/imagens/MPU6050.png)

---

