#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

void matmatijk(int LD, float A[][LD], float B [][LD], float C[][LD], int N);
void matmatjik(int LD, float A[][LD], float B [][LD], float C[][LD], int N);
void matmatikj(int LD, float A[][LD], float B [][LD], float C[][LD], int N);
void matmatkij(int LD, float A[][LD], float B [][LD], float C[][LD], int N);
void matmatkji(int LD, float A[][LD], float B [][LD], float C[][LD], int N);
void matmatjki(int LD, float A[][LD], float B [][LD], float C[][LD], int N);

void printMatrix(int LD, float M[][LD], int N);
void randomMatrix(int LD, float A[][LD], float B[][LD], float C[][LD], int N);

int main(int argc, const char* argv[]) {
	srand((unsigned int)time(NULL));
	int LD=500, N=100;
	float **A=NULL, **B=NULL, **C=NULL;
	double inizio, fine;

	A = (float**) malloc(LD * LD * sizeof(float));
	B = (float**) malloc(LD * LD * sizeof(float));
	C = (float**) malloc(LD * LD * sizeof(float));

	printf("\n");

	randomMatrix(LD, (float (*)[LD]) A, (float (*)[LD]) B, (float (*)[LD]) C, N);
	
	/*printf("A:\n");
	printMatrix(LD, (float (*)[LD]) A, N);
	printf("\nB:\n");
	printMatrix(LD, (float (*)[LD]) B, N);*/

	inizio = get_cur_time();
	matmatijk(LD, (float (*)[LD]) A, (float (*)[LD]) B, (float (*)[LD]) C, N);
	fine = get_cur_time();
	printf("IJK - Elapsed time: %lfs\n", fine - inizio);

	inizio = get_cur_time();
	matmatjik(LD, (float (*)[LD]) A, (float (*)[LD]) B, (float (*)[LD]) C, N);
	fine = get_cur_time();
	printf("JIK - Elapsed time: %lfs\n", fine - inizio);
	
	inizio = get_cur_time();
	matmatikj(LD, (float (*)[LD]) A, (float (*)[LD]) B, (float (*)[LD]) C, N);
	fine = get_cur_time();
	printf("IKJ - Elapsed time: %lfs\n", fine - inizio);

	inizio = get_cur_time();
	matmatjki(LD, (float (*)[LD]) A, (float (*)[LD]) B, (float (*)[LD]) C, N);
	fine = get_cur_time();
	printf("JKI - Elapsed time: %lfs\n", fine - inizio);

	inizio = get_cur_time();
	matmatkij(LD, (float (*)[LD]) A, (float (*)[LD]) B, (float (*)[LD]) C, N);
	fine = get_cur_time();
	printf("KIJ - Elapsed time: %lfs\n", fine - inizio);

	inizio = get_cur_time();
	matmatkji(LD, (float (*)[LD]) A, (float (*)[LD]) B, (float (*)[LD]) C, N);
	fine = get_cur_time();
	printf("KJI - Elapsed time: %lfs\n", fine - inizio);
		
	/*printf("\n***************\n\nC:\n");
	printMatrix(LD, (float (*)[LD]) C, N);*/

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

void randomMatrix(int LD, float A[][LD], float B[][LD], float C[][LD], int N) {
	int i, j, a=0, b=0;
	for(i=0; i<N; i++) {
		for(j=0; j<N; j++) {
			a = rand() % 100 + 1;
			b = rand() % 100 + 1;
			A[i][j]=a;
			B[i][j]=b;
			C[i][j]=0;
		}
	}
}

void matmatijk(int LD, float A[][LD], float B[][LD], float C[][LD], int N) {
	int i=0, j=0, k=0;
	for(i=0; i<N; i++)
		for(j=0; j<N; j++)
			for(k=0; k<N; k++)
				C[i][j] += A[i][k] * B[k][j];
}

void matmatjik(int LD, float A[][LD], float B[][LD], float C[][LD], int N) {
	int i=0, j=0, k=0;
	for(j=0; j<N; j++)
		for(i=0; i<N; i++)
			for(k=0; k<N; k++)
				C[i][j] += A[i][k] * B[k][j];
}

void matmatikj(int LD, float A[][LD], float B[][LD], float C[][LD], int N) {
	int i=0, j=0, k=0;
	for(i=0; i<N; i++)
		for(k=0; k<N; k++)
			for(j=0; j<N; j++)
				C[i][j] += A[i][k] * B[k][j];
}

void matmatkij(int LD, float A[][LD], float B[][LD], float C[][LD], int N) {
	int i=0, j=0, k=0;
	for(k=0; k<N; k++)
		for(i=0; i<N; i++)
			for(j=0; j<N; j++)
				C[i][j] += A[i][k] * B[k][j];
}

void matmatkji(int LD, float A[][LD], float B[][LD], float C[][LD], int N) {
	int i=0, j=0, k=0;
	for(k=0; k<N; k++)
		for(j=0; j<N; j++)
			for(i=0; i<N; i++)
				C[i][j] += A[i][k] * B[k][j];
}

void matmatjki(int LD, float A[][LD], float B[][LD], float C[][LD], int N) {
	int i=0, j=0, k=0;
	for(j=0; j<N; j++)
		for(k=0; k<N; k++)
			for(i=0; i<N; i++)
				C[i][j] += A[i][k] * B[k][j];
}
