#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "c_timer.h"

int main(int argc, const char* argv[]) {
	srand((unsigned int)time(NULL));
	int LDA=0, LDB=0, LDC=0, N=0, M=0, P=0, NTROW=0, NTCOL=0, dimension=0;
	double inizio, fine;
	
	void matmatthread(int LDA, int LDB, int LDC, float A[][LDA], float B [][LDB], float C[][LDC], int N, int M, int P, int NTROW, int NTCOL), 			nullifyMatrix(int LD, float M[][LD], int N),
		printMatrix(int LD, float M[][LD], int N),
		randomMatrix(int LD, float M[][LD], int N);

	if(argc == 4 &&
		atoi(argv[2]) <= 8 &&
		atoi(argv[3]) <= 8 &&
		(atoi(argv[1]) % atoi(argv[2]) == 0) &&
		(atoi(argv[1]) % atoi(argv[3]) == 0)) {
		dimension = atoi(argv[1]);
		NTROW = atoi(argv[2]);
		NTCOL = atoi(argv[3]);
	}
	else {
		perror("./mt [LD] [NTROW] [NTCOL]");
		return 0;
	}

	LDA = LDB = LDC = N = M = P = dimension;

	float *A = (float *) malloc(LDA * LDA * sizeof(float));
	float *B = (float *) malloc(LDB * LDB * sizeof(float));
	float *C = (float *) malloc(LDC * LDC * sizeof(float));
	
	randomMatrix(LDA, (float (*)[LDA]) A, LDA);
	randomMatrix(LDB, (float (*)[LDB]) B, LDB);
	nullifyMatrix(LDC, (float (*)[LDC]) C, LDC);

	inizio = get_cur_time();
	matmatthread(LDA, LDB, LDC, (float (*)[LDA]) A, (float (*)[LDB]) B, (float (*)[LDC]) C, N, M, P, NTROW, NTCOL);
	fine = get_cur_time();
	printf("\nProcess elapsed time: %lfs\n", fine - inizio);

	free(A);
	free(B);
	free(C);
	
	return 0;
}

void printMatrix(int LD, float M[][LD], int N) {
	int i, j;
	for(i=0; i<N; i++) {
		for(j=0; j<N; j++) {
			printf("[%2.1f] ", M[i][j]);
		}
		printf("\n");
	}
}

void randomMatrix(int LD, float M[][LD], int N) {
	int i, j, m=0;
	for(i=0; i<N; i++) {
		for(j=0; j<N; j++) {
			m = rand() % 100 + 1;
			M[i][j]=m;
		}
	}
}

void nullifyMatrix(int LD, float M[][LD], int N) {
	int i, j;
	for(i=0; i<N; i++) {
		for(j=0; j<N; j++) {
			M[i][j]=0;
		}
	}
}

void matmatikj(int LDA, int LDB, int LDC, float A[][LDA], float B[][LDB], float C[][LDC], int N, int M, int P) {
	int i=0, j=0, k=0;
	for(i=0; i<N; i++)
		for(k=0; k<M; k++)
			for(j=0; j<P; j++)
				C[i][j] += A[i][k] * B[k][j];
}

void matmatthread(int LDA, int LDB, int LDC, float A[][LDA], float B [][LDB], float C[][LDC], int N, int M, int P, int NTROW, int NTCOL) {
	int n_threads = NTROW * NTCOL, i=0, j=0;
	pthread_t **threads = (pthread_t **) malloc(NTROW * sizeof(pthread_t *));
	void *thread_function(void *arg);
	
	typedef struct {
		int row, column, LDA, LDB, LDC, N, M, P;
		float *A, *B, *C;
	} parameters;

	for(i=0; i < NTROW; i++)
		threads[i] = (pthread_t *) malloc(NTCOL * sizeof(pthread_t));

	N /= NTROW;
	P /= NTCOL;

	parameters *param;
	for(i=0; i < NTROW; i++) {
		for(j=0; j < NTCOL; j++) {
			param = (parameters *) malloc(sizeof(parameters));
			param->row = i;
			param->column = j;
			param->LDA = LDA;
			param->LDB = LDB;
			param->LDC = LDC;
			param->N = N;
			param->M = M;
			param->P = P;
			param->A = *A;
			param->B = *B;
			param->C = *C;
			pthread_create(&threads[i][j], NULL, thread_function, param);
		}
	}
	for(i=0; i < NTROW; i++) {
		for(j=0; j < NTCOL; j++) {
			pthread_join(threads[i][j], NULL);
		}
	}
}

void *thread_function(void *arg) {
	void matmatikj(int LDA, int LDB, int LDC, float A[][LDA], float B[][LDB], float C[][LDC], int N, int M, int P);
	typedef struct {
		int row, column, LDA, LDB, LDC, N, M, P;
		float *A, *B, *C;
	} parameters;

	parameters *param = (parameters *) arg;
	int row = param->row,
		column = param->column,
		LDA = param->LDA,
		LDB = param->LDB,
		LDC = param->LDC,
		N = param->N,
		M = param->M,
		P = param->P;

	float *A = param->A,
		*B = param->B,
		*C = param->C;
	
	double inizio, fine;

	float *myA = (float *) A + row * N * LDA,
			*myB = (float *) B + column * P,
			*myC = (float *) C + row * N * LDC + column * P;

	inizio = get_cur_time();
	matmatikj(LDA, LDB, LDC, (float (*)[LDA]) myA, (float (*)[LDB]) myB, (float (*)[LDC]) myC, N, M, P);
	fine = get_cur_time();
	printf("\nThread elapsed time: %lfs", fine - inizio);

	return 0;
}
