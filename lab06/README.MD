# Computação Concorrente 2022.1
**Willian Gomes Pessoa** **116210965**

***

## Laboratório 6

**Objetivo:** Alterar código de "Leitores e Escritores" de forma que os escritores tenham prioridade na escrita (leitores devem esperar qualquer escritor que estiver esperando para escrever passarem na frente)


### **Alterações realizadas**:

Inseri um contador de quantas threads querem escrever (chegaram no ponto de escrita e estão aguardando por algum motivo).

Uma leitura só pode ser iniciada se:

(1) Não há threads escrevendo;
(2) Não há threads escritoras aguardando liberação.


Trecho do código:
```
void InicLeit(int id) {
  [...]
  // Não realiza leitura enquanto houverem:
  // (1) Threads escrevendo
  // (2) Threads aguardando para escrever
  while (escr > 0 || quer_escrever > 0) { 
  [...]
```


### **5 Escritoras e 1 Leitora**:

Situação de propridade ocorrendo:

```
Escritora 4 esta escrevendo
E[2] quer escrever
E[2] bloqueou
L[1] quer ler
L[1] bloqueou, pois tem gente escrevendo
E[4] terminou de escrever
L[1] desbloqueou
L[1] bloqueou, pois tem 3 querendo escrever
E[5] desbloqueou
Escritora 5 esta escrevendo
E[5] terminou de escrever
L[1] desbloqueou
L[1] bloqueou, pois tem 2 querendo escrever
E[3] desbloqueou
Escritora 3 esta escrevendo
E[3] terminou de escrever
L[1] desbloqueou
L[1] bloqueou, pois tem 1 querendo escrever
E[2] desbloqueou
Escritora 2 esta escrevendo
E[2] terminou de escrever
L[1] desbloqueou
Leitora 1 esta lendo
L[1] terminou de ler
```

***
### **10 Escritoras e 1 Leitora**:

Situação de propridade ocorrendo:

```
L[1] quer ler
L[1] bloqueou, pois tem gente escrevendo
E[9] quer escrever
E[9] bloqueou
E[6] terminou de escrever
L[1] desbloqueou
L[1] bloqueou, pois tem 6 querendo escrever
E[3] desbloqueou
Escritora 3 esta escrevendo
E[3] terminou de escrever
L[1] desbloqueou
L[1] bloqueou, pois tem 5 querendo escrever
E[4] desbloqueou
Escritora 4 esta escrevendo
E[4] terminou de escrever
L[1] desbloqueou
L[1] bloqueou, pois tem 4 querendo escrever
E[7] desbloqueou
Escritora 7 esta escrevendo
E[7] terminou de escrever
L[1] desbloqueou
L[1] bloqueou, pois tem 3 querendo escrever
E[10] desbloqueou
Escritora 10 esta escrevendo
E[10] terminou de escrever
E[8] desbloqueou
Escritora 8 esta escrevendo
L[1] desbloqueou
L[1] bloqueou, pois tem gente escrevendo
E[8] terminou de escrever
E[9] desbloqueou
Escritora 9 esta escrevendo
L[1] desbloqueou
L[1] bloqueou, pois tem gente escrevendo
E[9] terminou de escrever
L[1] desbloqueou
Leitora 1 esta lendo
L[1] terminou de ler
```
