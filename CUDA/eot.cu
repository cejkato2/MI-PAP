#include <cuda.h>
#include <driver_types.h>
#include <cuda_runtime_api.h>
#include "eot.cuh"
#include "utils/utils.cuh"
#include "constant.h"
#include <math.h>
#include <utils/cuda_syn_block.cu> //synchronizace napric barierama -- prasarna vkladat primo ceckovy kod, ale :
//You must define __device__ functions within the compilation unit they are called in, and their behavior similar to functions declared with the c++ inline keyword.

// compare and swap; copies from the f to t, swapping f[i] and
// f[j] if the higher-index value is smaller; it is required that i < j
__device__ void cas(int *f, int *t, int i, int j, int n, int my_global_pos, int me)
{
	if (i < 0 || j >= n) return;
	if (me == i) {
		if (f[i] > f[j]) t[me] = f[j];
		else t[me] = f[i];
	} else { // me == j
		if (f[i] > f[j]) t[me] = f[i];
		else t[me] = f[j];
	}
}

// does one iteration of the sort
/*
da - ukazatel od prvniho pole
daaux - ukazatel do temp pole
n - velikost trideneho pole
iter - aktualni iterace
barnos - pomocne pole pro synchro mezi bloky
*/
__global__ void oekern(int *da, int *daaux, int n, int iter, volatile unsigned int* barnos)
{
int tix=threadIdx.x;
int d_index=blockIdx.x*NUM_OF_THREADS + tix; //globalni index v poli v hlavni pameti

__syncblocks(barnos);

	if( (iter%2) == 1){
	//provadej LS vymenu
		if( (tix%2) == 0){
			cas(da,daaux,d_index,d_index+1,n,d_index,d_index);
		}else{
			cas(da,daaux,d_index-1,d_index,n,d_index,d_index);
		}
	}else{
	//provadej SL vymenu
		if( (tix%2) == 1){
			cas(da,daaux,d_index,d_index+1,n,d_index,d_index);
		}else{
			cas(da,daaux,d_index-1,d_index,n,d_index,d_index);
		}
	}
}


void oddeven(int *ha, int n)
{
	int *da;
	int dasize = n * sizeof(int);

	HANDLE_ERROR(cudaMalloc((void **)&da, dasize));
	HANDLE_ERROR(cudaMemcpy(da, ha, dasize, cudaMemcpyHostToDevice));
	// the array daaux will serve as "scratch space"
	int *daaux;
	HANDLE_ERROR(cudaMalloc((void **)&daaux, dasize));

	int numOfBlocks = (int) ceil(ARRAY_SIZE/NUM_OF_THREADS); //number of blocks
	
	// ===== alokuj pole pro synchro =====
	unsigned int* h_barnos = new unsigned int[numOfBlocks]; //pomocne pole
	//inicializuj pole cislem 0
	for(int i=0;i<numOfBlocks;i++)	h_barnos[i] = 0;
	
	volatile unsigned int* barnos; //nase promenna, kterou budem ladovat cudu
	HANDLE_ERROR(cudaMalloc((void **) &barnos, sizeof(int)*numOfBlocks)); //alokujem si na to mistecko
	HANDLE_ERROR(cudaMemcpy((void*)barnos,h_barnos, sizeof(int)*numOfBlocks,cudaMemcpyHostToDevice)); //naládujem pole
	
	delete[] h_barnos; //a uklidime po sobe	pole,co nepotrebujeme				

	// ===== priprav sturkturu pro deleni problemu =====
	dim3 dimGrid(numOfBlocks, 1);
	dim3 dimBlock(NUM_OF_THREADS, 1, 1);

	//deme na problem	
	for (int iter = 1; iter <= n; iter++) {
		oekern <<< dimGrid, dimBlock >>> (da, daaux, n, iter,barnos); //one iteration
		cudaThreadSynchronize();
		if (iter < n) {	
			cudaMemcpy(da,daaux,dasize,cudaMemcpyDeviceToDevice); //refresh copy
		} else{
			cudaMemcpy(ha, daaux, dasize, cudaMemcpyDeviceToHost); //copy results
		}
	}
	//free malocs
	HANDLE_ERROR(cudaFree(da));
	HANDLE_ERROR(cudaFree(daaux));
}

