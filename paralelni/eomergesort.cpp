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

void oddEvenMergeSort2(int *a, int size, int lo, int n)
{
	std::vector<State *> stack;
	std::vector<State *> queue;
	stack.push_back(new State(lo, n));
	State *current_state = NULL;

	while (stack.empty() != true) {
		//get state from the top of the stack and remove it
		current_state = stack.back();
		queue.insert(queue.begin(), current_state);
		stack.pop_back();

		//std::cout << current_state->start_index << " " << current_state->num << std::endl;

		if (current_state->num > 1) {
			int m = current_state->num / 2;
			stack.push_back(new State(current_state->start_index + m, m));
			stack.push_back(new State(current_state->start_index, m));
		}
	}

	std::vector<State *>::iterator it;
	for (it = queue.begin(); it != queue.end(); ++it) {
		std::cout << (*it)->start_index << " " << (*it)->num << std::endl;
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

	// threads = sum of threads (>=1)
	// !!! it has to be 2^n, for any n from N + {0} !!!
	if ((int)pow(2, ((int)log2(threads))) != threads) {
		std::cerr << "!!! Amount of threads has to be 2^n, for any n from N + {0}, or 1 !!!" <<
		std::endl;
		return 1;
	}

	timer = omp_get_wtime();

	#ifdef DEBUG_OUTPUT
	std::cout << "Vstupní pole:\n" << std::endl;
	a.print();
	std::cout << "\n";
	#endif

	//oddEvenMergeSort(a.getData(), a.getSize(), 0, a.getSize());
	//return 0;
	//a.print();

	int lgsize = (int)log2(a.getSize());
	if ((int)pow(2, lgsize) != a.getSize()) {
		std::cerr << "Array size is not multiple of 2" << std::endl;
	}



	// for threads == 1 it will be sequential, whole array at once
	// otherwise it is reduced size
	int red_size = a.getSize() / threads;
	int *arr_part = NULL;

	//std::cout << "total size: " << a.getSize() << std::endl;
	#pragma omp parallel for default(shared) schedule(static,1) firstprivate(arr_part, red_size) num_threads(threads)
	for (int i = 0; i < threads; ++i) {
		//std::cout << "thread array: " << (i*red_size) << " " << (i*red_size) + red_size << std::endl;
		arr_part = a.getData() + (i * red_size);
		// sort subsequence
		oddEvenMergeSort(arr_part, red_size, 0, red_size);
	}





	//merge ordered subsequences

	int index = 1;


	for (int subseq = (threads / 2); subseq > 0; subseq /= 2) {

		int partsize = a.getSize() / subseq;
		lgsize = (int)log2(partsize);

		int part = 0;
		int i = 0;

//# pragma omp parallel for default(shared) schedule(dynamic) firstprivate(i, lgsize, index) num_threads(threads)
		for (part = 0; part < subseq; ++part) {
			int *subarray = a.getData() + (partsize * part);

			for (i = 0; i < lgsize; ++i) {
				index = (int)pow(2, i);
				oddEvenMerge(subarray, partsize, index - 1, partsize, index);
			}
		}


		//functional for 2 threads
		//for (int i=0; i<lgsize; ++i) {
		//  index = (int) pow(2,i);
		//  oddEvenMerge(a.getData(), a.getSize(), index - 1, a.getSize(), index);
		//}

	}

	std::cout << "Elapsed time: " << omp_get_wtime() - timer << "s" << std::endl;
	//a.print();
	testSortedArraysekv(a);

	//a.print();

	#ifdef DEBUG_OUTPUT
	std::cout << "Setříděné pole:\n" << std::endl;
	a.print();
	#endif


	//oddEvenMergeSort2(a.getData(), a.getSize(), 0, a.getSize(), threads);

	return 0;
}


