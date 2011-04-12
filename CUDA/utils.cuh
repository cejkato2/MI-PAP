#ifndef UTILS_CUH_
#define UTILS_CUH_

#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

#include <stdio.h>

/*
*Error handlign function
*/
static void HandleError( cudaError_t err, const char *file, int line ) {
	if (err != cudaSuccess) {
	printf( "%s in %s at line %d\n", cudaGetErrorString( err ), file, line );
	exit( EXIT_FAILURE );
	}
}


#endif
