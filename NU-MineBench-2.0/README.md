## Observações quanto à implementação ScalParcC:
- Comparei com o programa do RMS-TM e lá as memórias transacionais são utilizadas para substituir as seções críticas;
- Usei a clausula `transaction` como definido no nosso conceito, identificando somente a aplicação para uma variável (ver arquivo parclass.c);
- Algumas das seções críticas são utilizadas para calcular os *chunks* dos laços. As mesmas poderiam ser removidas (a exemplo do que fiz no arquivo drive.c) utilizando `parallel for` que calcula automaticamente o *chunk* para cada thread que vai executar o laço. Porém, achei que isso não se enquadra na nossa proposta.

## Observações quanto à implementação Apriori e UtilityMine:
- Os programas são bem mais complexos que o ScalParcC;
- Comparei com os programas do RMS-TM e lá as memórias transacionais são utilizadas para substituir locks do OpenMP utilizados dentro de funções e métodos de uma classe HashTree. Vou continuar estudando os códigos, mas a princípio não entendi direito o funcionamento das transações nele. Assim, não sei como isso se encaixaria na nossa proposta;
- Usei a clausula `transaction` como definido no nosso conceito, identificando somente a aplicação para uma variável no **Apriori** (ver arquivo no_output_parapr.C) e para três variáveis no **UtilityMine** (ver arquivo utility.cc); 
