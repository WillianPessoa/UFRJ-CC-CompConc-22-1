/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: Inicializa um vetor com 10000 mil elementos, onde os valores de cada posição é o seu índice.
           Eleva cada elemento do vetor ao quadrado.
           Verifica se todos os elementos foram elevados ao quadrado.
           Todos os passos acima são feitos utilizando duas threads  */

//TODO: Colocar esse código para funcionar com um número indefinido de threads
//TOCHECK: Existe a possibilidade de verificarmos quantos nucleos/threads reais existem na máquina?

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS  2 // Total de threads a serem criadas
#define SIZE 10000 // Elementos do vetor 

int myVector[10000];

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, nThreads;
} t_Args;

// Inicializa os elementos do vetor 
void inicializaVetor() {
  int i = 0;
  for (i = 0; i < SIZE; ++i) {
    myVector[i] = i;
  }
}

// Verifica se os elementos do vetor foram elevados ao quadrado
void verificaVetor() {
  
  int i = 0;
  int hasError = 0; 
  for (i = 0; i < 5000; ++i) {
    if (!myVector[i] == (i*i)) {
      printf("O elemento %d do vetor nao foi elevado ao quadrado. O valor encontrado foi '%d' e deveria ser %d.\n", i, myVector[i], i*i);
      hasError = 1;
    }
  }

  if (!hasError) {
    printf("\nTodos os elementos do vetor foram elevados ao quadrado.");
  } else {
    printf("\nErros ocorreram durante a execucao do programa e pelo menos um elemento nao foi elevado ao quadrado.\n");
  }
}

// funcao executada pelas threads
void *elevaAoQuadrado (void *arg) {
  t_Args *args = (t_Args *) arg;

  int i = 0;
  if (args->idThread == 0) {
      for (i = 0; i < 5000; ++i) {
          myVector[i] = myVector[i]*myVector[i];
      }
  } else {
      for (i = 5000; i < 10000; ++i) {
          v[i] = myVector[i]*myVector[i];
      }
  }

  pthread_exit(NULL);
}

// funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar
  t_Args *arg; //receberá os argumentos para a thread

  inicializaVetor();

  /* ELEVANDO AO QUADRADO */
  for (thread = 0; thread < NTHREADS; ++thread) { // -- cria as threads
    printf("--Aloca e preenche argumentos para thread %d\n", thread);
    arg = malloc(sizeof(t_Args));
    if (arg == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    arg->idThread = thread;
    arg->nThreads = NTHREADS;


    printf("--Cria a thread %d\n", thread);
    if (pthread_create(&tid_sistema[thread], NULL, elevaAoQuadrado, (void*) arg)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }

  }

  for (thread=0; thread < NTHREADS; thread++) { //--espera todas as threads terminarem
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1);
    }
  }


  /* Verificando */
  verificaVetor();

  printf("\n--Thread principal terminou\n");
  pthread_exit(NULL);
}
