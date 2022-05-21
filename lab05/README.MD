# Computação Concorrente 2022.1
**Willian Gomes Pessoa** **116210965**

***

## Laboratório 5

**Objetivo:** Entender e aplicar os conceitos de variáveis de condições da biblioteca Pthread.


### **Atividade 1**:

A aplicação entra em deadlock e, consequentemente, nunca termina quando o número de threads A é alterado para 1.

***

### **Atividade 2**:

Sim! Como o `pthread_cond_broadcast` libera todas as threads que estão aguardando/bloqueadas, a condição lógica vai sempre ser atendida.

### **Atividade 3**:

O logo de execução foi sempre correto.

Como x pode ser incrementado entre essas duas linhas, não podemos realizar essa subtituição. Pois tal incremento faria x não ser um múltilo de 10.

Ocorreram erros no log ao inverter as linhas 50 e 51: valores não múltiplos de 10 foram impressos.

### **Atividade 4**:

Necessário verificar o código em `main.c`. Mas foi obtido a ordem correta das mensagens em todos os testes realizados.