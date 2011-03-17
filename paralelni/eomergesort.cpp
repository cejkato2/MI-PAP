#include <math.h>
#include <omp.h>
#include <iostream>
#include "../array.h"
#include "../utils/2Darray.h"
#include "../utils/loader.h"

void oddEvenMergeSort(int *a, int size, int lo, int n);
void oddEvenMerge(int *a, int size, int lo, int n, int r);
void compare(int *a, int i, int j);
void exchange(int *a, int i, int j);

void oddEvenMergeSort(int *a, int size, int lo, int n)
{
//std::cout << "oddEvenMergeSort(" << lo << ", " << n << ")" << std::endl;
	if (n > 1) {
		int m = n / 2;
		oddEvenMergeSort(a, size, lo, m);
		oddEvenMergeSort(a, size, lo + m, m);
		oddEvenMerge(a, size, lo, n, 1);
	}
}

/** lo is the starting position and
 *  n is the length of the piece to be merged,
 *  r is the distance of the elements to be compared
 */
void oddEvenMerge(int *a, int size, int lo, int n, int r)
{
//std::cout << "oddEvenMerge(" <<  lo << ", " <<  n << ", " <<  r << ")" << std::endl;
	int m = r * 2;

	if (m < n) {
		oddEvenMerge(a, size, lo, n, m);        // even subsequence
		oddEvenMerge(a, size, lo + r, n, m);    // odd subsequence
		for (int i = lo + r; i + r < lo + n; i += m) {
			//if ((i + r) < size) {
			compare(a, i, i + r);
			//}
		}
	} else {
		//if ((lo + r) < size) {
		compare(a, lo, lo + r);
		//}
	}
}

void compare(int *a, int i, int j)
{
	if (a[i] > a[j])
		exchange(a, i, j);
}

void exchange(int *a, int i, int j)
{
	//std::cout << "exchange(" << i << ", " << j << ")" << std::endl;
	int t = a[i];

	a[i] = a[j];
	a[j] = t;
}

int main(int argc, char** argv)
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

	#ifdef DEBUG_OUTPUT
	std::cout << "Vstupní pole:\n" << std::endl;
	a.print();
	std::cout << "\n";
	#endif

	oddEvenMergeSort(a.getData(), a.getSize(), 0, a.getSize());

	#ifdef DEBUG_OUTPUT
	std::cout << "Setříděné pole:\n" << std::endl;
	a.print();
	#endif

	std::cout << "Elapsed time: " << omp_get_wtime() - timer << "s" << std::endl;

	return 0;
}


