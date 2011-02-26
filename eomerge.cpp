#include <iostream>
#include "mergesort.h"

int main(int argc, char **argv)
{
	int a[6] = { 10, 6, 4, 8, 3, 20 };
	int aux[6];

	mergeSort(a, aux, 0, 5);

	for (int i = 0; i < 6; i++) {
		std::cout << a[i] << " ";
	}
	std::cout << std::endl;

	return 0;
}
