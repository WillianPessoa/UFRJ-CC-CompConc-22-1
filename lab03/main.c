/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 3 */
/* Código: Encontrar o menor e o maior valor de um vetor através de um código executado concorrentemente (com threads)*/
#include <float.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "timer.h"

// Variáveis global para o vetor
double *vector;

/**
 * @brief Estrutura auxiliar utilizada nas threads
 */
 typedef struct {
	 int id;
	 long long int size;
	 int step;
	 double minor;
	 double major;
 }concorrentData;

/** 
 * @brief Imprime elementos do vetor (não valida os parâmetros)
 * @param vector Vetor com os elementos
 * @param size Tamanho do vetor
 */
void print(double *vector, long long int size) {
	printf("Printing a vector with size %lld: ", size);
	for (long long int i = 0; i < size; ++i) {
		printf("%.2f ", vector[i]);
	}
	puts("");
}

/** 
 * @brief Encontra o menor e maior valor (não valida os parâmetros)
 * @param vector Vetor com os elementos
 * @param size Tamanho do vetor
 * @param minor Variável passada por referência para armazenar MENOR valor
 * @param major Variável passada por referência para armazenar MAIOR valor
 */
void findMinorAndMajorSequential(double *vector, long long int size, double *minor, double *major)
{
	*minor = vector[0];
	*major = vector[0];
	for (long long int i = 1; i < size; ++i) {
		if (vector[i] > *major) {
			*major = vector[i];
		} 
		else if (vector[i] < *minor) {
			*minor = vector[i];
		}
	}
}

void *findMinorAndMajorConcurrent(void *arg)
{
	// Convertendo argumento
	concorrentData* args = (concorrentData*) arg;

	// Realizando varredura
	for (long long int i = args->id; i < args->size; i += args->step) {
		// Primeira iteração
		if (i == args->id) {
			args->major = vector[0];
			args->minor = vector[0];
		} else {// Verificando menores e maiores
			if (vector[i] < args->minor) {
				args->minor = vector[i];
			} 
			else if (vector[i] > args->major) {
				args->major = vector[i];
			}
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]) 
{
	// Obtendo parâmetros
    if (argc < 3) {
        printf("Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }

	long long int size = atoll(argv[1]);
	int nthreads = atoi(argv[2]);

	/* Inicializando variáveis */

	// Inicializando rand
	srand(time(0));

	// Vetor
	vector = (double*) malloc(sizeof(double) * size);
	if (!vector) { printf("ERRO--malloc\n"); return 2; }
	for (long long int i = 0; i < size; ++i) {
		vector[i] = rand() * 1.1;
	}

	// Id da thread
	pthread_t *threadIds = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
	if (!threadIds) { printf("ERRO--malloc\n"); return 3; }

	// Estrutura para versão concorrente
	concorrentData *args = (concorrentData*) malloc(sizeof(concorrentData) * size);
	if (!args) { printf("ERRO--malloc\n"); return 4; }

	// Variáveis para armazenar min e max em sequencial
	double minorSequential = DBL_MAX;
	double majorSequential = DBL_MIN;

	// Controle de tempo
	double start = -1;
    double end = -1;
    double deltaSequential = -1;
    double deltaConcurrent = -1;

	// Imprime vetor
	// print(vector, size);

	// Calcula tempo de operação sequencial
	GET_TIME(start);
	findMinorAndMajorSequential(vector, size, &minorSequential, &majorSequential);
	GET_TIME(end);
	deltaSequential = end - start;

	printf("Minor value founded in vector by sequential code: %lf\n", minorSequential);
	printf("Major value founded in vector by sequential code: %lf\n", majorSequential);
	printf("Time spent: %.6lf\n\n", deltaSequential);

	// Calcula tempo de operação concorrente
	GET_TIME(start);
	
	// Para cada thread
	for (int i = 0; i < nthreads; ++i) {
		(args + i)->id = i;
		(args + i)->size = size;
		(args + i)->step = nthreads;
		// Cria as threads usando a função concorrente
		if (pthread_create((threadIds + i), NULL, findMinorAndMajorConcurrent, (void*) (args+i))) {
			printf("ERRO--pthread_create\n");
			break;
		}
	}

	// Variáveis para armazenar min e max em concorrente
	double minorConcurrent = DBL_MAX;
	double majorConcurrent = DBL_MIN;

	// Aguarda o término de cada thread
	for (int i = 0; i < nthreads; ++i) {
		pthread_join(*(threadIds + i), NULL);
		
		// Verificando se a thread atual achou um menor
		if ((args + i)->minor < minorConcurrent) {
			minorConcurrent = (args + i)->minor;
		}
		// Verificando se a thread atual achou um maior
		if ((args + i)->major > majorConcurrent) {
			majorConcurrent = (args + i)->major;
		}
	}

	GET_TIME(end);
	deltaConcurrent = end - start;

	printf("Minor value founded in vector by concurrent code: %lf\n", minorConcurrent);
	printf("Major value founded in vector by concurrent code: %lf\n", majorConcurrent);
	printf("Time spent: %.6lf\n\n", deltaConcurrent);

	// Libera memória
	free(vector);
	free(args);
	return 0;
}