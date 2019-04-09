# Implementação OpenMP para memórias transacionais (experimental)

## O que há por aqui?
Arquivos C++ com as (pseudo) implementações em OpenMP utilizando a cláusula `transaction` para controlar o acesso a variáveis dentro das regiões paralelas

Fontes disponíveis em no diretório cowichan/cowichan_openmp_transaction

### Problemas Cowichan
Implementações disponíveis do diretório **cowichan/openmp_transaction**.

#### Benchmarks com implementação realizada:
- hull
- norm
- outer
- sor
- thresh
- vecdiff
- winnow (somente quick_sort)

#### Fontes originais do cowichan:
Disponível em: https://code.google.com/archive/p/cowichan/

Obs.: analisar demais benchmarks, pois a princípio não há necessidade de implementação

### NU-MineBench-2.0
Implementações disponíveis do diretório **NU-MineBench-2.0/src**.

#### Benchmarks com implementação realizada (inicial):
- Apriori
- ScalParC
- UtilityMine

#### Fontes originais do cowichan:
Disponível em: http://cucis.ece.northwestern.edu/projects/DMS/MineBenchDownload.html
