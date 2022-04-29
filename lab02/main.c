#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <stdbool.h>
#include"timer.h"

// Matrizes
int *input_matrix1;
int *input_matrix2;
int *output_matrix;

// Quantidade de threads
int nthreads;

/** @Brief Estrutura a ser passada para as outras threads. */
typedef struct{
    int id; // Identificador da thread
    int dim; // Tamanho da matriz
} tArgs;


/**
 * @Brief Realiza a multiplicação de duas matrizes quadradas de forma sequêncial
 * @param matrix01 Ponteiro para a primeira matriz
 * @param matrix02 Ponteiro para a segunda matriz
 * @param result Ponteiro para a matriz de resultado
 * @param size Tamanho da matriz */
void sequential_multiplier(int *matrix01, int *matrix02, int* result, int size)
{
    // Tratamentos de erro
    if (!matrix01) {
        puts("A primeira matriz foi informada com um ponteiro nulo");
        return;
    }
    
    if (!matrix01) {
        puts("A segunda matriz foi informada com um ponteiro nulo");
        return;
    }
    
    if (!result) {
        puts("A matriz de resultado foi informada com um ponteiro nulo");
        return;
    }
    
    if (size <= 1) {
        puts("Valor inválido para uma matriz");
        return;
    }
    
    // Realizando multiplicação
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            result[ (i*size)+j ] = matrix01[ (i*size)+j ] *
                                   matrix02[ (i*size)+j ];
        }
    }
    
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                result[ i*size+j ] = input_matrix1[i*size + k] * 
                                     input_matrix2[k*size + j];
            }
        }
    }
}

/**
 * @Brief Realiza a multiplicação de duas matrizes quadradas de forma concorrente
 * @param arg Ponteiro para tArgs* (contém identificador da thread e tamanho da matriz) */
void *concurrent_multiplier(void *arg)
{
    // Obtém parâmetro da estrutura tArgs
    tArgs *args = (tArgs*) arg;
    
    // Realizando multiplicação
    for(int i=args->id; i<args->dim; i+=nthreads){
        for(int j=0; j<args->dim; j++){
            for (int k = 0; k < args->dim; k++)
            {
            output_matrix[i*args->dim + j] = input_matrix1[i*args->dim + k] * 
                                             input_matrix2[k*args->dim + j];
            }
            
        }
    }
         
   pthread_exit(NULL);
}

/** 
 * @Brief Preenche uma matriz com um determinado valor (se o valor for -1, preenche com valores randômicos)
 * @param matrix Matriz a ser preenchida
 * @param size Tamanho da matriz
 * @param value Valor a ser preenchido na matriz (se -1, preenche com valores randômicos) */
void fill(int *matrix, int size, int value)
{
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[ (i*size)+j ] = value;
        }
    }
}

/**
 * @Brief Imprime uma matriz
 * @param matrix Matriz a ser impressa
 * @param size Tamanho da matriz */
void print(int *matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            printf("%d ", matrix[ (i*size)+j ]);
        }
        printf("\n");
    }
    printf("\n");
}



/** @Brief Ponto de entrada. */
int main(int argc, char* argv[]) {
    
    // Verificando parâmetros
    if (argc < 3) {
        printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
        return 1;
    }
    int size = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > size) nthreads = size;
    
    // Alocando
    input_matrix1 = (int *) malloc(sizeof(int) * size * size);
    input_matrix2 = (int *) malloc(sizeof(int) * size * size);
    output_matrix = (int *) malloc(sizeof(int) * size * size);
    if (!input_matrix1 || !input_matrix2 || !output_matrix) {
        printf("ERRO--malloc\n");
        return 2;
    }
    
    // Prenchendo
    fill(input_matrix1, size, 1);
    fill(input_matrix2, size, 1);
    fill(output_matrix, size, 0); // Não é necessário
    
    // Imprimindo
    /*puts("Matriz 1:\n");
    print(input_matrix1, size);
    puts("");
    
    puts("Matriz 2:\n");
    print(input_matrix2, size);
    puts("");*/
    
    // Variáveis para obtenção do tempo
    double start = -1;
    double end = -1;
    double delta_sequential = -1;
    double delta_concurrent = -1;
    
    /** @Sequential Realizando multiplicação sequêncial */
    GET_TIME(start);
    sequential_multiplier(input_matrix1, input_matrix2, output_matrix, size);
    GET_TIME(end);
    delta_sequential = end - start;
    printf("Tempo da multiplicação sequencial: %.5f\n", delta_sequential);
    
    /*puts("Matriz resultante:\n");
    print(output_matrix, size);
    puts("");*/
    
    /** @Concurrent Realizando multiplicação concorrente */
    GET_TIME(start);
    // Alocação das Estruturas
    pthread_t *tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    tArgs *args = (tArgs*) malloc(sizeof(tArgs) * nthreads);
    if (!tid || !args) {
        printf("ERRO--malloc\n");
        return 3;
    }
    
    // Chama as threads para executar a função de multiplicação
    for(int i=0; i<nthreads; i++) {
        (args+i)->id = i;
        (args+i)->dim = size;
        if(pthread_create(tid+i, NULL, concurrent_multiplier, (void*) (args+i))){
            puts("ERRO--pthread_create"); return 3;
        }
    } 

    // Espera pelo termino das threads
    for (int i = 0; i < nthreads; i++){
        pthread_join(*(tid + i), NULL);
    }
    
    GET_TIME(end);
    delta_concurrent = end - start;
    printf("Tempo da multiplicação concorrente: %.5f\n", delta_concurrent);
    
    /*puts("Matriz resultante:\n");
    print(output_matrix, size);
    puts("");*/
    
    return 0;
}
    
