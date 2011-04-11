// Utilities and system includes
#include <cuda.h>
#include <driver_types.h>
#include <cuda_runtime_api.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 8
#define MODULE 100

#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

/*
*Error handlign function
*/
static void HandleError( cudaError_t err, const char *file, int line ) {
	if (err != cudaSuccess) {
	printf( "%s in %s at line %d\n", cudaGetErrorString( err ), file, line );
	exit( EXIT_FAILURE );
	}
}


// compare and swap; copies from the f to t, swapping f[i] and
// f[j] if the higher-index value is smaller; it is required that i < j
__device__ void cas(int *f, int *t, int i, int j, int n, int me)
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
	int bix = blockIdx.x; // block number within grid
	if (iter % 2) {
		if (bix % 2) cas(da, daaux, bix - 1, bix, n, bix);
		else cas(da, daaux, bix, bix + 1, n, bix);
	} else {
		if (bix % 2) cas(da, daaux, bix, bix + 1, n, bix);
		else cas(da, daaux, bix - 1, bix, n, bix);
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
	dim3 dimGrid(n, 1);
	dim3 dimBlock(1, 1, 1);
	int *tmp;
	for (int iter = 1; iter <= n; iter++) {
		oekern <<< dimGrid, dimBlock >>> (da, daaux, n, iter);
		cudaThreadSynchronize();
		if (iter < n) {	
		// swap pointers
			tmp = da;
			da = daaux;
			daaux = tmp;
		} else{
			cudaMemcpy(ha, daaux, dasize, cudaMemcpyDeviceToHost); //copy results
		}
	}
	//free malocs
	HANDLE_ERROR(cudaFree(da));
	HANDLE_ERROR(cudaFree(daaux));
}


void generateArray(int a[],int size)
{
	for(int i=0;i<size;i++){
	  a[i]=(rand() % MODULE);
	}
}


void printArray(int a[],int size)
{
	for(int i=0;i<size;i++){
		printf("%d ",a[i]);
	}
	printf("\n");
}

int main(int argc, char** argv)
{
cudaDeviceProp prop; //vlastnosti gpu
int whichDev; //device

int* a = new int[ARRAY_SIZE];

generateArray(a, ARRAY_SIZE);
printArray(a,ARRAY_SIZE);

//HANDLE_ERROR(cudaGetDevice(&whichDev));
//HANDLE_ERROR(cudaGetDeviceProperties(&prop,whichDev));

//printf("MultiProcCount->%d\n",prop.multiProcessorCount);
return 0;
}
