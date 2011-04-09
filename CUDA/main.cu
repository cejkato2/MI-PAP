#include <cuda.h>
#include <driver_types.h>
#include <cuda_runtime_api.h>
#include <stdio.h>

#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

/*
*Error handlig function
*/
static void HandleError( cudaError_t err, const char *file, int line ) {
	if (err != cudaSuccess) {
	printf( "%s in %s at line %d\n", cudaGetErrorString( err ), file, line );
	exit( EXIT_FAILURE );
	}
}



int main(int argc, char** argv)
{
cudaDeviceProp prop; //vlastnosti gpu
int deviceCount; //device

HANDLE_ERROR(cudaGetDeviceCount(&deviceCount));
printf("%d GPU CUDA device(s) found\n", deviceCount);
return 0;
}
