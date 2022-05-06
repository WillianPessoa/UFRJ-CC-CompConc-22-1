# Computação Concorrente 2022.1
**Willian Gomes Pessoa** **116210965**

***

## Laboratório 3

**Objetivo:** Implementar uma solução sequencial e uma concorrente para o problema de encontrar o **menor** e o **maior** valor de um vetor de números reais.


### 10^5

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 0.000261 | 0.001006 | 0.259443
2 | 0.000261 | 0.000523 | 0.499043
4 | 0.000261 | 0.000498 | 0.524093

***

### 10^7

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 0.025552 | 0.044636 | 0.572452
2 | 0.025552 | 0.025067 | 1.019348
4 | 0.025552 | 0.012320 | 2.074025

***

### 10^8

Numero de threads | Tempo sequencial | Tempo concorrente | Desempenho
----------------- |------------------|-------------------|------------
1 | 0.255689 | 0.363179 | 0.704030
2 | 0.255689 | 0.201133 | 1.271243
4 | 0.255689 | 0.118582 | 2.158750