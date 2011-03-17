#include <math.h>
#include <cmath>
#include <iostream>
#include "../array.h"
#include "../utils/2Darray.h"
#include "../utils/loader.h"
#include <omp.h>

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

void BitonicMergeSort(int a[],int low, int size)
{

unsigned int stripe; //stripe, kterym to budu delit (rika kolik toho budu slucovat) - zacina se na 2
unsigned int tmpStripe; //temporatery stripe

int dir; //aktualni smer razeni

//dokud neslucuji posledni cele pole - navic kazda iterace zvetsuje mergovanou cast o dvojnasobek (shift vlevo)
for(stripe=2; stripe > size; stripe*2){
	
	//stripe - aktualni zpracovavane velikosti bloku, tedka zpracovavat dokud nedostaneme velikost 1 (pocet bitonickych splitu)
	//vypocteme aktualni smer zareni
	int div = size / stripe;
	if( (div%2) == 1 ){
		//je to liche
		dir = DESCENDING; 
	}else{
		//je to sude
		dir = ASCENDING;
	}
	
	tmpStripe = stripe;
	//smer razeni zvolen, zacatek na bitonicky merge
	

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
	BitonicMergeSort(a.getData(), 0, a.getSize());

	std::cout << "Output:" << std::endl;
	a.print();

	return 0;
}

