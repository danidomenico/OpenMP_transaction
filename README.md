# cowichan - Implementação OpenMP para memórias transacionais (experimental)

#### O que há por aqui?
Arquivos C++ com as (pseudo) implementações em OpenMP dos problemas Cowichan utilizando a cláusula transaction para controlar o acesso a variáveis dentro das regiões paralelas

Fontes disponíveis em no diretório cowichan/cowichan_openmp_transaction

#### Fontes originais do cowichan
Disponível em: https://code.google.com/archive/p/cowichan/

#### Benchmarks com implementação realizada
- hull
- norm
- outer
- sor
- thresh
- vecdiff
- winnow (somente quick_sort)

Obs.: analisar demais benchmarks, pois a princípio não há necessidade de implementação
