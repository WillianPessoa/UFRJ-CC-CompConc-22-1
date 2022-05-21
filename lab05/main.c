/*Disciplina: Computacao Concorrente */
/*Prof.: Silvana Rossetto */
/*Módulo 1 - Laboratório: 5 */
/*Código: Projetar e implementar uma aplicação com 5 threads que imprima as mensagens conforme regras explicitadas no PDF do laboratório 5 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Identificadores das threads
enum ThreadsIds {
  ComeAgain,
  MakeYourselfAtHome,
  PleaseSitDown,
  GlassOfWater,
  Welcome  
};

// Variáveis globais
const int nthreads = 5;

int threadsExecutedCount = 0;

pthread_mutex_t mutex;
pthread_cond_t initial_condition; // -> Thread 5 imprimiu
pthread_cond_t other_condition; // -> 4 threads imprimiram

void waitInitialCondition(){
    pthread_mutex_lock(&mutex);
    while (threadsExecutedCount < 1){
        pthread_cond_wait(&initial_condition, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void *messageComeAgain(void *arg) 
{
    pthread_mutex_lock(&mutex);
    if (threadsExecutedCount < 4){
        pthread_cond_wait(&other_condition, &mutex);
    }
    printf("Volte sempre!\n");
    ++threadsExecutedCount;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *messageMakeYourselfAtHome(void *arg) 
{   
    waitInitialCondition();
    pthread_mutex_lock(&mutex);
    printf("Fique a vontade.\n");
    ++threadsExecutedCount;
    if (threadsExecutedCount == 4){
        pthread_cond_signal(&other_condition);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}   

void *messagePleaseSitDown(void *arg)
{
    waitInitialCondition();
    pthread_mutex_lock(&mutex);
    printf("Sente-se por favor.\n");
    ++threadsExecutedCount;
    if (threadsExecutedCount == 4){
        pthread_cond_signal(&other_condition);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
    pthread_exit(NULL);
}

void *messageGlassOfWater(void *arg)
{
    waitInitialCondition();
    pthread_mutex_lock(&mutex);
    printf("Aceita um copo de agua?\n");
    ++threadsExecutedCount;
    if (threadsExecutedCount == 4){
        pthread_cond_signal(&other_condition);
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
    pthread_exit(NULL);
}

void *messageWelcome(void *arg)
{
    pthread_mutex_lock(&mutex);
    printf("Seja bem vindo\n");
    ++threadsExecutedCount;
    pthread_cond_broadcast(&initial_condition);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // Quantidade de threads
    pthread_t threads[nthreads];

    // Inicialização do mutex e das variáveis condicionais
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&initial_condition, NULL);
    pthread_cond_init(&other_condition, NULL);

    // Inicialização das threads
    for (int i = 0; i < nthreads; ++i){
        void* (*fun_ptr)(void*) = NULL;
        switch(i) {

            case ComeAgain:
                fun_ptr = &messageComeAgain;
                break; 
                
            case MakeYourselfAtHome:
                fun_ptr = &messageMakeYourselfAtHome;
                break; 
                
            case PleaseSitDown:
                fun_ptr = &messagePleaseSitDown;
                break;
                
            case GlassOfWater:
                fun_ptr = &messageGlassOfWater;
                break;
                
            case Welcome:
                fun_ptr = &messageWelcome;
                break; 
            
            default:
                printf("A thread %d nao pode ser inicializada.\n", i);
        }
        pthread_create(&threads[i], NULL, fun_ptr, NULL);
    }

    // Aguarda a finalização das threads
    for (int i = 0; i < nthreads; i++){
        pthread_join(threads[i], NULL);
    }

    // Liberação do mutex e das variáveis condicionais
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&initial_condition);
    pthread_cond_destroy(&other_condition);

    return 0;
}
