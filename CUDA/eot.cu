#include <cuda.h>
#include <driver_types.h>
#include <cuda_runtime_api.h>
#include "eot.cuh"
#include "utils.cuh"
#include "constant.h"
#include <math.h>


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
__global__ void oekern(int *da, int *daaux, int n, int iter)
{
int tix=threadIdx.x;

	if( (iter%2) == 1){
	//provadej LS vymenu
	if( (tix%2) == 0){
		cas(da,daaux,tix,tix+1,n,n,tix);
		}else{
		cas(da,daaux,tix-1,tix,n,n,tix);
		}
	}else{
	//provadej SL vymenu
	if( (tix%2) == 1){
		cas(da,daaux,tix,tix+1,n,n,tix);
		}else{
		cas(da,daaux,tix-1,tix,n,n,tix);
		}
	}
//synchronizuj vlakna v bloku
	__syncthreads();
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

	dim3 dimGrid(numOfBlocks, 1);
	dim3 dimBlock(NUM_OF_THREADS, 1, 1);

	for (int iter = 1; iter <= n; iter++) {
		oekern <<< dimGrid, dimBlock >>> (da, daaux, n, iter); //one iteration
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

