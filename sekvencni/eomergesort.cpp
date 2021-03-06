#include <math.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include "../utils/array.h"
#include "../utils/2Darray.h"
#include "../utils/loader.h"
#include "../utils/sortTester.h"

struct State {
	int start_index;
	int num;
	State(int si, int n)
	{
		start_index = si;
		num = n;
	}
};

void oddEvenMergeSort(int *a, int size, int lo, int n, int threads);
void oddEvenMerge(int *a, int size, int lo, int n, int r);
void compare(int *a, int i, int j);
void exchange(int *a, int i, int j);

void oddEvenMergeSort(int *a, int size, int lo, int n)
{
	if (n > 1) {
		int m = n / 2;
		oddEvenMergeSort(a, size, lo, m);
		oddEvenMergeSort(a, size, lo + m, m);
		//std::cout << "oddEvenMergeSort(" << lo << ", " << n << ")" << std::endl;
		oddEvenMerge(a, size, lo, n, 1);
	}
}

/** lo is the starting position and
 *  n is the length of the piece to be merged,
 *  r is the distance of the elements to be compared
 */
void oddEvenMerge(int *a, int size, int lo, int n, int r)
{
	//std::cout << "1oddEvenMerge(" <<  lo << ", " <<  n << ", " <<  r << ")" << std::endl;
	int m = r * 2;

	if (m < n) {
		oddEvenMerge(a, size, lo, n, m);        // even subsequence
		oddEvenMerge(a, size, lo + r, n, m);    // odd subsequence
		//std::cout << "2oddEvenMerge(" <<  lo << ", " <<  n << ", " <<  r << ")" << std::endl;
		for (int i = lo + r; i + r < lo + n; i += m) {
			compare(a, i, i + r);
		}
	} else {
		compare(a, lo, lo + r);
	}
}

void compare(int *a, int i, int j)
{
	//std::cout << "compare " << i << " " << j << std::endl;
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


	timer = omp_get_wtime();

	#ifdef DEBUG_OUTPUT
	std::cout << "Vstupní pole:\n" << std::endl;
	a.print();
	std::cout << "\n";
	#endif

	oddEvenMergeSort(a.getData(), a.getSize(), 0, a.getSize());

	std::cout << "Elapsed time: " << omp_get_wtime() - timer << "s" << std::endl;
	testSortedArray(a, 2);

	#ifdef DEBUG_OUTPUT
	std::cout << "Setříděné pole:\n" << std::endl;
	a.print();
	#endif

	return 0;
}


