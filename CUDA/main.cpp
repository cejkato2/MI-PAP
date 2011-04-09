// Utilities and system includes
#include <cuda.h>
//#include <driver_types.h>
//#include <cuda_runtime_api.h>
#include <stdio.h>

//#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

/*
*Error handlign function
*/
//static void HandleError( cudaError_t err, const char *file, int line ) {
//	if (err != cudaSuccess) {
//	printf( "%s in %s at line %d\n", cudaGetErrorString( err ), file, line );
//	exit( EXIT_FAILURE );
//	}
//}



int main(int argc, char** argv)
{
//cudaDeviceProp prop; //vlastnosti gpu
int whichDev; //device

//HANDLE_ERROR(cudaGetDevice(&whichDev));
//HANDLE_ERROR(cudaGetDeviceProperties(&prop,whichDev));

return 0;
}
