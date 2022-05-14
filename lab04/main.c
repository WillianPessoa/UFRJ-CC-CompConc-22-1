/*Disciplina: Computacao Concorrente */
/*Prof.: Silvana Rossetto */
/*Módulo 1 - Laboratório: 4 */
/*Código: Dado um vetor de entrada de numeros inteiros, gerar um vetor de saída de numeros reais, calculando a raíz quadrada de todos os elementos do vetor de entrada que forem primos. */

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "timer.h"

// Variáveis global para o vetor
int *inputVector;
double *outputVectorSequential;
double *outputVectorConcurrent;
long long int concurrentGlobalIndex = 0;

// variável para sincronização de exclusão mutua
pthread_mutex_t lock;

/**
 *@brief Estrutura auxiliar utilizada nas threads
 */
typedef struct {
    int id;
    long long int size;
}ConcurrentData;

/**
 *@brief Verifica a primalidade de um número.
 *@param number Número a ser verificado.
 *@return 1 se é primo, 0 se não.
 */
int isPrime(long long int number){
    if (number <= 1){
        return 0;
    }
    if (number == 2){
        return 1;
    }
    if (number%2 == 0 || number%3 == 0){
        return 0;
    }
    for (int i = 5; i < sqrt(number)+1; i += 4){
        if (number%i == 0){
            return 0;
        }
        i += 2;
        if (number%i == 0){
            return 0;
        }
    }
    return 1;
}

/**
 *@brief Processa sequencialmente (da maneira pedida neste lab) todos os números do vetor de entrada.
 *@param inputVector Vetor de entrada.
 *@param outputVector Vetor de saída.
 *@param size Tamanho dos vetores.
 */
void processSequentially(int *inputVector, double *outputVector, long long int size)
{
    for (int i = 0; i < size; ++i) {
        if (isPrime(inputVector[i])) {
            outputVector[i] = sqrt(inputVector[i]);
        } else {
            outputVector[i] = inputVector[i];
        }
    }
}

/**
 *@brief Processa paralelamente (da maneira pedida neste lab) todos os números do vetor de entrada.
 *@param inputVector Vetor de entrada.
 *@param outputVector Vetor de saída.
 *@param size Tamanho dos vetores.
 */
void *processConcurrently(void * arg)
{
    ConcurrentData *concurrentData = (ConcurrentData*) arg;

    // Primeira sessão crítica
    pthread_mutex_lock(&lock);
    long long int currentIndex = concurrentGlobalIndex++;
    pthread_mutex_unlock(&lock);

    while(currentIndex < concurrentData->size) {
        if (isPrime(inputVector[currentIndex])) {
            outputVectorConcurrent[currentIndex] = sqrt(inputVector[currentIndex]);
        } else {
            outputVectorConcurrent[currentIndex] = inputVector[currentIndex];
        }

        // Segunda sessão crítica
        pthread_mutex_lock(&lock);
        currentIndex = concurrentGlobalIndex++;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

/**
 *@brief Verifica se os dois vetores de entrada iguais (possuem os mesmos valores)
 *@param first Primeiro vetor.
 *@param second Segundo vetor.
 *@return 1 se sim, 0 se não.
 */
int areEquals(double *first, double *second, long long int size)
{
    for (int i = 0; i < size; ++i) {
        if (first[i] != second[i]) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    // Obtendo parâmetros
    if (argc < 3)
    {
        printf("Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }

    long long int size = atoll(argv[1]);
    int nthreads = atoi(argv[2]);

    /* Inicialização das variáveis */

    // Inicializando rand
    srand(time(0));

    // Vetor de Entrada
    inputVector = (int*) malloc(sizeof(int) * size);
    if (!inputVector) {
        printf("ERRO--malloc\n");
        return 2;
    }
    for (int i = 0; i < size; ++i) {
        inputVector[i] = rand();
    }

    // Vetor de Saída Sequencial
    outputVectorSequential = (double*) malloc(sizeof(double) * size);
    if (!outputVectorSequential) {
        printf("ERRO--malloc\n");
        return 3;
    }

    // Vetor de Saída Concorrente
    outputVectorConcurrent = (double*) malloc(sizeof(double) * size);
    if (!outputVectorConcurrent) {
        printf("ERRO--malloc\n");
        return 4;
    }

    // Estruturas auxiliares
    // Note: Neste lab, não precisava. Mas usei mesmo assim para não colocar a variável size como global.
    ConcurrentData *concurrentData = (ConcurrentData*) malloc( sizeof(ConcurrentData) * nthreads);
    if (!concurrentData) {
        printf("ERRO--malloc\n");
        return 5;
    }

    // Id da thread
    pthread_t *threadIds = (pthread_t*) malloc(sizeof(pthread_t) *nthreads);
    if (!threadIds)
    {
        printf("ERRO--malloc\n");
        return 6;
    }

    // Inicialização de variável de exclusão mútua
    pthread_mutex_init(&lock, NULL);

    // Controle de tempo
    double start = -1;
    double end = -1;
    double deltaSequential = -1;
    double deltaConcurrent = -1;

    // Calcula tempo de operação sequencial
    GET_TIME(start);
    processSequentially(inputVector, outputVectorSequential, size);
    GET_TIME(end);
    deltaSequential = end - start;
    printf("Time spent for sequential code: %.6lf\n\n", deltaSequential);

    // Calcula tempo de operação concorrente
    GET_TIME(start);
    for (int i = 0; i < nthreads; ++i) {
        (concurrentData+i)->id = i;
        (concurrentData+i)->size = size;
        if (pthread_create((threadIds + i), NULL, processConcurrently, (void*) (concurrentData+i))) {
            printf("ERRO--pthread_create\n");
            return 7;
        }
    }

    for (int i = 0; i < nthreads; ++i) {
        pthread_join(*(threadIds +i), NULL);
    }

    GET_TIME(end);
    deltaConcurrent = end - start;
    printf("Time spent for concurrent code: %.6lf\n\n", deltaConcurrent);


    // Verifica se são iguais
    printf("The resulting vector of concurrent and sequential code ");
    if (areEquals(outputVectorSequential, outputVectorConcurrent, size)) {
        printf("ARE EQUAL!\n");
    } else {
        printf("ARE DIFFERENT!\n");
    }
    

    // Liberando memória
    if (!inputVector) free(inputVector);
    if (!outputVectorSequential) free(outputVectorSequential);
    if (!outputVectorConcurrent) free(outputVectorConcurrent);
    if (!concurrentData) free(concurrentData);
    if (!threadIds) free(threadIds);

    return 0;
}
