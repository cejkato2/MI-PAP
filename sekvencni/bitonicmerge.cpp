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
//void BitonicMergeSort(int a[], int low, int count, int direction)
//{
	//pokud je pocet prvku vetsi jak 1
//	if (count > 1) {
//		int m = count / 2;
//		BitonicMergeSort(a, low, m, neg(direction));
//		BitonicMergeSort(a, low + m, count - m, direction);
//		BitonicMerge(a, low, count, direction); //merge in direction
//	}
//}

void BitonicSortOperation(BitonicIndexRecord* brecord)
{

}


void BitonicMergeSort(int a[], int low, int count, int direction)
{
//priprava na vlozeni na stack prvniho zaznamu
BitonicIndexRecord* brecord = new BitonicIndexRecord();
brecord->low=low;
brecord->count=count;
brecord->direction=direction;
brecord->typeOfOperation=BITONIC_SORT; //operace je bitonicky sort

//dokud neni stack prazdny, tak
while(stack.size != 0){

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

