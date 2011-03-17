#include <stdio.h>
#include <omp.h>
#include <math.h>


int main(int argc, char** argv)
{

	int a[8] = { 10, 22, 1, 13, 2, 5, 0, 12 };


	for (int i = 0; i < 7; i++) {
		printf("%i ", a[i]);
	}
	printf("\n");


	for (int i = 0; i < 7; i++) {
		printf("%i ", a[i]);
	}
	printf("\n");

	return 0;
}

