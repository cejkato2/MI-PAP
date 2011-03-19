#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string.h>
#include <omp.h>
#include "../utils/mergesort.h"
#include "../utils/array.h"
#include "../utils/2Darray.h"
#include "../utils/loader.h"



int main(int argc, char **argv)
{
	//for time measurement:
	double timer;

	//for setting parameters:
	char *filename = NULL;
	int threads, array_size;

	//data container:
	Array a;

	getSettings(&filename, &array_size, &threads, argc, argv);
	loadValues(filename, a, array_size);

	if (a.isEmpty() == true) {
		std::cerr << "No input values" << std::endl;
		return 1;
	}

	int *aux = new int[a.getSize()];

	timer = omp_get_wtime();
	#ifdef DEBUG_OUTPUT
	std::cout << "Input array:" << std::endl;
	a.print();
	std::cout << std::endl;
	#endif

	mergeSort(a.getData(), aux, 0, a.getSize() - 1);

	#ifdef DEBUG_OUTPUT
	std::cout << "Sorted array:" << std::endl;
	a.print();
	std::cout << std::endl;
	#endif

	std::cout << "Elapsed time: " << omp_get_wtime() - timer << "s" << std::endl;

	delete [] aux;

	return 0;
}

