# Computação Concorrente 2022.1
**Willian Gomes Pessoa** **116210965**

***

## Laboratório 4

**Objetivo:** Dado um vetor de entrada de numeros inteiros, gerar um vetor de saída de numeros reais, calculando a raíz quadrada de todos os elementos do vetor de entrada que forem primos.


### 10^5

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 0.902420 | 0.901675 | 1,000826
2 | 0.902420 | 0.678743 | 1,329545
4 | 0.902420 | 0.419918 | 2,149038

***

### 10^7

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 98.573977 | 99.681726 | 0,988887
2 | 98.573977 | 56.694395 | 1,738689
4 | 98.573977 | 44.370749 | 2,221598

***

### 10^8

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 965.231587 | 962.943852 | 1,002375
2 | 965.231587 | 534.669565 | 1,805286
4 | 965.231587 | 389.061337 | 2,480923