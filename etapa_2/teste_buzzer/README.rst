Teste Buzzer -> Título


Inicialmente nossa pretenção é testar algumas funções (não todas) que serão utilziadas no nosso projeto, bem como um start point para os próximos desenvolvimentos utilizando o componente em questão.

No inicio foram criadas duas funções de buzzer para o teste:
- buzzer_1bip: Bip único para ser utilizado repetidamente duranta a "ultima fase" da máquina de estados, na qual um bip sonoro é emitido de tempos em tempos para encontrarmos o foguete.
- buzzer_DoneInit: Sequencia de 3 Bips para identificar que a inicialização dos periféricos está pronta, e que podemos prosseguir com o lançamento. Será útilizado no estado de pré-lançamento.

Para a elaboração foi utilizado o VS-CODE com a extensão do ESP-IDF.
