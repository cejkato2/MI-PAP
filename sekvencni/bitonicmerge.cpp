#include <math.h>
#include <stack>
#include <iostream>
#include "../array.h"
#include "../utils/2Darray.h"
#include "../utils/loader.h"

#define ASCENDING 0
#define DESCENDING 1

#define BITONIC_SORT 0
#define BITONIC_MERGE 1

//struktura, ktera pujde na stack
struct BitonicIndexRecord {
	int low;
	int count;
	int direction;
	int typeOfOperation;
	int* a; //pole na predani
};

std::stack<BitonicIndexRecord* > stack;

//funkce bitonickeho sortu

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

void BitonicMergeOperation(BitonicIndexRecord* brecord)
{
	int count = brecord->count;

	if (count > 1) {
		int* a =  brecord->a;
		int m = greatestPowerOfTwoLessThan(count);
		int low = brecord->low;
		int direction = brecord->direction;
		//bitonic split
		for (int i = low; i < low + count - m; i++) {
			CompareAndExchange(a, i, m + i, direction);
		}

		//vytvoreni zaznamu
		BitonicIndexRecord* bmerge1, *bmerge2;

//BitonicIndexRecord* brecord = new BitonicIndexRecord();
		bmerge1 = new BitonicIndexRecord();
		bmerge1->low = low;
		bmerge1->count = m;
		bmerge1->direction = direction;
		bmerge1->typeOfOperation = BITONIC_MERGE;
		bmerge1->a = a;

		bmerge2 = new BitonicIndexRecord();
		bmerge2->low = low + m;
		bmerge2->count = count - m;
		bmerge2->direction = direction;
		bmerge2->typeOfOperation = BITONIC_MERGE;
		bmerge2->a = a;

		stack.push(bmerge2);
		stack.push(bmerge1);
	}
}

int neg(int direction)
{
	if (direction == ASCENDING) {
		return DESCENDING;
	}
	return ASCENDING;
}


void BitonicSortOperation(BitonicIndexRecord* brecord)
{
	int count = brecord->count;

	if (count > 1) {
		int m = count / 2;
		int low = brecord->low;
		int direction = brecord->direction;
		int* a = brecord->a;
		//priprava struktur na zasobnik
		BitonicIndexRecord* bitonic1, *bitonic2, *bmerge;
		//priprava struktur
		bitonic1 = new BitonicIndexRecord();
		bitonic1->low = low;
		bitonic1->count = m;
		bitonic1->direction = neg(direction);
		bitonic1->typeOfOperation = BITONIC_SORT;
		bitonic1->a = a;

		bitonic2 = new BitonicIndexRecord();
		bitonic2->low = low + m;
		bitonic2->count = count - m;
		bitonic2->direction = direction;
		bitonic2->typeOfOperation = BITONIC_SORT;
		bitonic2->a = a;

		bmerge = new BitonicIndexRecord();
		bmerge->low = low;
		bmerge->count = count;
		bmerge->direction = direction;
		bmerge->typeOfOperation = BITONIC_MERGE;
		bmerge->a = a;

		//nahazu to na zasobnik
		stack.push(bmerge);
		stack.push(bitonic2);
		stack.push(bitonic1);
	}

}


void BitonicMergeSort(int a[], int low, int count, int direction)
{
//priprava na vlozeni na stack prvniho zaznamu
	BitonicIndexRecord* brecord = new BitonicIndexRecord();

	brecord->low = low;
	brecord->count = count;
	brecord->direction = direction;
	brecord->typeOfOperation = BITONIC_SORT; //operace je bitonicky sort
	brecord->a = a;

	stack.push(brecord);

//dokud neni stack prazdny, tak
	while ((int)stack.size() != 0) {
		brecord = stack.top();
		stack.pop();
//podle typu zaznamu zavolej prislusnou funkci
		if (brecord->typeOfOperation == BITONIC_SORT) {
			BitonicSortOperation(brecord);
		}


		if (brecord->typeOfOperation == BITONIC_MERGE) {
			BitonicMergeOperation(brecord);
		}

		delete brecord; //delete value
	}

}

int main(int argc, char** argv)
{
	char *filename = NULL;
	Array a;


	getFilename(&filename, argc, argv);
	loadValues(filename, a);

	if (a.isEmpty() == true) {
		std::cerr << "No input values" << std::endl;
		return 1;
	}


	std::cout << "Input:" << std::endl;

	a.print();
	BitonicMergeSort(a.getData(), 0, a.getSize(), ASCENDING);

	std::cout << "Output:" << std::endl;
	a.print();

	return 0;
}

