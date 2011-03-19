#include <stdio.h>
#include <math.h>


/**
 * Slevani pro Merge sort
 * @param array pole k serazeni
 * @param aux pomocne pole (stejne velikosti jako razene)
 * @param left prvni index, na ktery smim sahnout
 * @param right posledni index, na ktery smim sahnout
 */
void merge(int array[], int aux[], int left, int right)
{
	int middleIndex = (left + right) / 2;
	int leftIndex = left;
	int rightIndex = middleIndex + 1;
	int auxIndex = left;

	while (leftIndex <= middleIndex && rightIndex <= right) {
		if (array[leftIndex] <= array[rightIndex]) {
			aux[auxIndex] = array[leftIndex++];
		}else{
			aux[auxIndex] = array[rightIndex++];
		}
		auxIndex++;
	}
	while (leftIndex <= middleIndex) {
		aux[auxIndex] = array[leftIndex++];
		auxIndex++;
	}
	while (rightIndex <= right) {
		aux[auxIndex] = array[rightIndex++];
		auxIndex++;
	}

	for (int i = left; i <= right; i++) {
		array[i] = aux[i];
	}
}

/** Razeni slevanim (od nejvyssiho)
 * @param array pole k serazeni
 * @param aux pomocne pole stejne delky jako array
 * @param left prvni index na ktery se smi sahnout
 * @param right posledni index, na ktery se smi sahnout
 */
void mergeSort(int array[], int aux[], int left, int right)
{
	if (left == right) return;
	int middleIndex = (left + right) / 2;
	mergeSort(array, aux, left, middleIndex);
	mergeSort(array, aux, middleIndex + 1, right);
	merge(array, aux, left, right);

}

#ifdef DEBUG_MERGESORT
int main(int argc, char** argv)
{

	int a[6] = { 10, 6, 4, 8, 3, 20 };
	int aux[6];

	mergeSort(a, aux, 0, 5);

	for (int i = 0; i < 6; i++) {
		printf("%i ", a[i]);
	}
	printf("\n");

	return 0;
}
#endif

