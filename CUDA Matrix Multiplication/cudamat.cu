#include <stdio.h>
#include <stdlib.h>

extern "C" {
	#include "c_timer.h"
}

#define BLOCK_WIDTH 32

int main(int argc, char* argv[]) {
	float *A=NULL, *B=NULL, *C=NULL, *A_d=NULL, *B_d=NULL, *C_d=NULL;
	int i=0, N=0, M=0, P=0, nbrow=0, nbcol=0;
	double inizio, fine;

	__global__ void matmat(float *, float *, float *, int, int, int);
	void printMatrix(float *, const char[], int, int);

	if(argc == 6) {
		N = atoi(argv[1]);
		M = atoi(argv[2]);
		P = atoi(argv[3]);
		nbrow = atoi(argv[4]);
		nbcol = atoi(argv[5]);
	} else if(argc == 4) {
		N = atoi(argv[1]);
		M = atoi(argv[2]);
		P = atoi(argv[3]);
	} else {
		fprintf(stderr, " %s N M P [nbrow] [nbcol] -- [] means optional", argv[0]);
		perror(" ");
		return 0;
	}

	A = (float *) malloc(N * M * sizeof(float));
	B = (float *) malloc(M * P * sizeof(float));
	C = (float *) malloc(N * P * sizeof(float));

	cudaMalloc((void **) &A_d, N * M * sizeof(float));
	cudaMalloc((void **) &B_d, M * P * sizeof(float));
	cudaMalloc((void **) &C_d, N * P * sizeof(float));

	for(i=0; i < N * M; i++)
		*(A + i) = i+1;
	for(i=0; i < M * P; i++)
		*(B + i) = i+1;
	for(i=0; i < N * P; i++)
		*(C + i) = 0;

	cudaMemcpy(A_d, A, N * M * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(B_d, B, M * P * sizeof(float), cudaMemcpyHostToDevice);
	cudaMemcpy(C_d, C, N * P * sizeof(float), cudaMemcpyHostToDevice);

	dim3 DimGrid(1, 1);
	dim3 DimBlock(1, 1);

	if(argc == 4) {
		nbrow = ceil((double)N/BLOCK_WIDTH);
		nbcol = ceil((double)P/BLOCK_WIDTH);
		DimGrid.x = nbcol;
		DimGrid.y = nbrow;
		DimBlock.x = BLOCK_WIDTH;
		DimBlock.y = BLOCK_WIDTH;
	} else {
		nbrow += N%nbrow;
		nbcol += P%nbcol;
		DimGrid.x = nbcol;
		DimGrid.y = nbrow;
		DimBlock.x = P/nbcol;
		DimBlock.y = N/nbrow;
	}

	inizio = get_cur_time();
	matmat<<<DimGrid, DimBlock>>>(A_d, B_d, C_d, N, M, P);
	cudaDeviceSynchronize();
	fine = get_cur_time();
	printf(" GPU Computation Time: %lfs\n", fine - inizio);
	printf(" GPU Gflop/s: %e\n", ((double)2*M*N*P)/(fine - inizio));
	printf("\n");

	cudaMemcpy(C, C_d, N * P * sizeof(float), cudaMemcpyDeviceToHost);

	/*printMatrix(A, "A", N, M);
	printf("\n");
	printMatrix(B, "B", M, P);
	printf("\n");
	printMatrix(C, "C", N, P);
	printf("\n");*/

	free(A);
	free(B);
	free(C);
	cudaFree(A_d);
	cudaFree(B_d);
	cudaFree(C_d);

	return 0;
}

__global__ void matmat(float *A_d, float *B_d, float *C_d, int N, int M, int P) {
	int k=0, row = blockIdx.y * blockDim.y + threadIdx.y, col = blockIdx.x * blockDim.x + threadIdx.x;

	if(row < N && col < P) {
		for(k=0; k < M; k++)
			C_d[row * P + col] += A_d[row * M + k] * B_d[k * P + col];
		//printf(" %d|%d %d|%d C(%d,%d):%.2f\n", blockIdx.y, blockIdx.x, threadIdx.y, threadIdx.x, row, col, C_d[row * P + col]);
	}
}

void printMatrix(float *M, const char name[], int row, int col) {
	int i;
	printf(" %s:", name);
	for(i=0; i < row * col; i++) {
		if(i % col == 0) printf("\n");
		printf("  %.2f ", *(M + i));
	}
	printf("\n");
}
