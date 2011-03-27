#ifndef MERGESORT_H

//konstanty pro smer trideni
#define ASCENDING 0
#define DESCENDING 1

//konstanty pro pristup k poli (radkovy a sloupcovy pristup)
#define ROW 0
#define COLUMN 1

/*!
 * Implementation of merge sort
 * @param array[] data
 * @param aux[] temporary array
 * @param left left index of array for sorting
 * @param right right index of array for sorting
 */
void mergeSort(int array[], int aux[], int left, int right, int fixedPoint, int arrayAcces, int dir);

#endif
