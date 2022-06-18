# Computação Concorrente 2022.1
**Willian Gomes Pessoa** **116210965**

***

## Laboratório 7

**Objetivo:** Implementar um programa concorrente em Java para encontrar a quantidade de números pares em um vetor de inteiros . Divida a tarefa entre as threads de forma balanceada e garanta ausência de condição de corrida.


### **Observações sobre a solução**:

Mesmo colocando `syncronized` em todos os métodos da minha classe de estrutura de dados, ainda houveram acessos a determinados métodos quando o índice do vetor já se encontrava maior ou igual ao tamanho do vetor. 

Sendo assim, apenas consegui atender a condição de corrida colocando uma verificação dentro destes métodos para verificar se o índice era válido. Quando não, retornava null e a thread tinha que ignorá-lo consequentemente e se encerrar (o encerramento não depende desse retorno).

