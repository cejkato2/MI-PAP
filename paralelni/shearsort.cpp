#include <iostream>
#include <cmath>
#include <cstdlib>
#include <omp.h>
#include "../utils/2Darray.h"
#include "../utils/array.h"
#include "../utils/loader.h"
#include "shearmergesort.h"

int getRowDirection(int row)
{
	if (row % 2 == 0) {
		return ASCENDING;
	}else{
		return DESCENDING;
	}
}

void shearSort(Array2D& a, int rows, int cols, int numOfThreads)
{
	int numOfPhases = 2 * ((int)floor(log2(rows))) + 1;
	int* auxRow = new int[rows];
	int* auxCols = new int[cols];

	#ifdef DEBUG_OUTPUT
	std::cout << "Počet fází:" << numOfPhases << std::endl;
	//0 - je prvni liche cislo
	//1 - je prvni sude cislo
	#endif
	int row, col;
	for (int phase = 0; phase < numOfPhases; phase++) {

		if ((phase % 2) == 0) {
			//licha faze - razeni radku
			for (row = 0; row < rows; row++) {
				//proved MergeSort na radkovy vektor v danem smeru razeni s pritupem po radcich 
				mergeSort(a, auxRow, 0, cols-1, row, ROW, getRowDirection(row));
			}
		}else{
			//suda faze - razeni sloupcu
			for (col = 0; col < cols; col++) {
				//proved MergeSort na sloupcovy vektor v vzestupnem razeni s pristupem po sloupcich
				mergeSort(a, auxCols, 0, rows-1, col, COLUMN, ASCENDING);
			}
		}
	}
}



void generateTable(Array2D& a, int x, int y);

int main(int argc, char** argv)
{
	//for time measurement:
	double timer;

	//for setting parameters:
	char *filename = NULL;
	int threads, array_size;

	//data container:
	Array2D a(0, 0);

	getSettings(&filename, &array_size, &threads, argc, argv);
	loadValues(filename, &a, array_size);

	if (a.getSize() == 0) {
		std::cerr << "No input values" << std::endl;
		return 1;
	}

	timer = omp_get_wtime();

	#ifdef DEBUG_OUTPUT
	std::cout << "Vstupní pole:\n" << std::endl;
	a.print();
	std::cout << "\n";
	#endif

	shearSort(a, a.getSizeX(), a.getSizeY(), threads);

	#ifdef DEBUG_OUTPUT
	std::cout << "Setříděné pole:\n" << std::endl;
	a.print();
	#endif

	std::cout << "Elapsed time: " << omp_get_wtime() - timer << "s" << std::endl;

	return 0;
}

void generateTable(Array2D& a, int x, int y)
{
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			int num = (rand() % 100) + 1;
			a.setValueAt(num, i, j);
		}
	}
}

