#include <stdio.h>
#include <math.h>

#define ASCENDING 0
#define DESCENDING 1

int greatestPowerOfTwoLessThan(int n)
{
	int k = 1;

	while (k < n) {
		k = k << 1;
	}
	return k >> 1;
}


void CompareAndExchange(int a[], int i, int j, int dir)
{
	if (a[i] < a[j]) {
		if (dir == DESCENDING) {
			//pokud je sestupny smer =>prohod; vim, ze prvekk na i je mensi jak na j
			int tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
		}
	}else{
		if (dir == ASCENDING) {
			//pokud je vzestupny smer=>prohod; vim, ze prvek na j je mensi jak na i
			int tmp = a[i];
			a[i] = a[j];
			a[j] = tmp;
		}
	}
}


void BitonicMerge(int a[], int low, int count, int direction)
{
	if (count > 1) {
		int m = greatestPowerOfTwoLessThan(count);
		for (int i = low; i < low + count - m; i++) {
			CompareAndExchange(a, i, m + i, direction);
		}

		BitonicMerge(a, low, m, direction);             //opakuj to same na prvni
		BitonicMerge(a, low + m, count - m, direction); //a druhou polovinu
	}
}

int neg(int direction)
{
	if (direction == ASCENDING) {
		return DESCENDING;
	}
	return ASCENDING;
}

//low = spodni index
void BitonicMergeSort(int a[], int low, int count, int direction)
{
	//pokud je pocet prvku vetsi jak 1
	if (count > 1) {
		int m = count / 2;
		BitonicMergeSort(a, low, m, neg(direction));
		BitonicMergeSort(a, low + m, count - m, direction);
		BitonicMerge(a, low, count, direction); //merge in direction
	}
}


int main(int argc, char** argv)
{

	int a[8] = { 10, 22, 1, 13, 2, 5, 0, 12 };


	for (int i = 0; i < 7; i++) {
		printf("%i ", a[i]);
	}
	printf("\n");

	BitonicMergeSort(a, 0, 7, ASCENDING);

	for (int i = 0; i < 7; i++) {
		printf("%i ", a[i]);
	}
	printf("\n");

	return 0;
}

