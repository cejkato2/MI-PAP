#include <iostream>
#include <cmath>
#include "../utils/2Darray.h"
#include "eot.h"
#include <cstdlib>

int getRowDirection(int row)
{
	if (row % 2 == 0) {
		return ASCENDING;
	}else{
		return DESCENDING;
	}
}

void shearSort(Array2D& a, int rows, int cols)
{
	int numOfPhases = 2 * ((int)floor(log2(rows))) + 1;

	std::cout << "Počet fází:" << numOfPhases << std::endl;
//0 - je prvni liche cislo
//1 - je prvni sude cislo
	for (int phase = 0; phase < numOfPhases; phase++) {
		if ((phase % 2) == 0) {
			//licha faze
			for (int row = 0; row < rows; row++) {
				EOTRow(a, cols, row, getRowDirection(row)); //kazdy radek v jimem smeru
			}
		}else{
			//suda faze
			for (int col = 0; col < cols; col++) {
				EOTColumn(a, rows, col, ASCENDING); //serad vsechny sloupce smerem dolu
			}
		}
	}
}


void generateTable(Array2D& a, int x, int y);

int main(int argc, char** argv)
{

	Array2D a(10, 10);

	std::cout << "Vstupní pole:\n" << std::endl;
	generateTable(a, 10, 10);
	a.print();
	std::cout << "\n";

	shearSort(a, 10, 10);

	std::cout << "Setříděné pole:\n" << std::endl;
	a.print();

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

