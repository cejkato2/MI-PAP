#include <iostream>
#include <fstream>
#include <getopt.h>
#include <string.h>
#include "mergesort.h"
#include "array.h"
#include "utils/2Darray.h"
#include "utils/loader.h"



int main(int argc, char **argv)
{
	char *filename = NULL;
	Array a;


	getFilename(&filename, argc, argv);
	loadValues(filename, a);

	if (a.isEmpty() == true) {
		std::cerr << "No input values" << std::endl;
		return 1;
	}


	a.print();

	return 0;
}
