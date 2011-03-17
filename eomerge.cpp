#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string.h>
#include <omp.h>
#include "mergesort.h"
#include "array.h"
#include "utils/2Darray.h"
#include "utils/loader.h"



int main(int argc, char **argv)
{
	double timer;
	char *filename = NULL;
	Array a;


	timer = omp_get_wtime();
	getFilename(&filename, argc, argv);
	loadValues(filename, a);

	if (a.isEmpty() == true) {
		std::cerr << "No input values" << std::endl;
		return 1;
	}


	int *aux = new int[a.getSize()];

	mergeSort(a.getData(), aux, 0, a.getSize() - 1);


	std::cout << "Elapsed time: " << omp_get_wtime() - timer << "s" << std::endl;
	a.print();

	delete [] aux;

	return 0;
}

