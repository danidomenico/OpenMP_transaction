## Observações quanto à implementação ScalParcC:
- Comparei com o programa do RMS-TM e lá as memórias transacionais são utilizadas para substituir as seções críticas;
- Usei a clausula `transaction` como definido no nosso conceito, identificando somente a aplicação para uma varível (ver arquivo parclass.c);
- Algumas das seções críticas são utilizadas para calcular os *chunks* dos laços. As mesmas poderiam ser removidas (a exemplo do que fiz no arquivo drive.c) utilizando `parallel for` que calcula automaticamente o *chunk* para cada thread que vai executar o laço. Porém, achei que isso não se enquadra na nossa proposta.

