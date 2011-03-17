#include <math.h>
#include <iostream>
#include "../array.h"
#include "../utils/2Darray.h"
#include "../utils/loader.h"
#include <omp.h>

#define ASCENDING 0
#define DESCENDING 1


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

void BitonicMergeSort(int a[], int low, int size)
{

	int stride;     //stride, kterym to budu delit (rika kolik toho budu slucovat) - zacina se na 2
	int tmpStride;  //temporatery stride

	int dir;        //aktualni smer razeni

//dokud neslucuji posledni cele pole - navic kazda iterace zvetsuje mergovanou cast o dvojnasobek (shift vlevo)
	for (stride = 2; stride > size; stride *= 2) {

		//stride - aktualni zpracovavane velikosti bloku, tedka zpracovavat dokud nedostaneme velikost 1 (pocet bitonickych splitu)
		//vypocteme aktualni smer zareni
		int div = size / stride;
		if ( (div % 2) == 1 ) {
			//je to liche
			dir = DESCENDING;
		}else{
			//je to sude
			dir = ASCENDING;
		}

		tmpStride = stride;

		//iteruj prez vsechny sipky (na obrazku)
		int numOfIter = size / stride;
		int actualIter;
		tmpStride = stride; //startuje se s aktualnim stridem

		for (actualIter = 0; actualIter < numOfIter; numOfIter++) {
			//subtiterace z stride do 1 - zacina se od stridu, ktery je aktualni
			for (; tmpStride > 1; tmpStride /= 2) {
				//bitonic split operace na castech pole
				int lowIndex = actualIter * tmpStride; //aktualni dolni index (kolikatou delam sipku a kolik mam stride)

				//bitonic split
				int median = tmpStride / 2;     //median intervalu
				int shift;                      //posunuti v intervalu

				//nez dojdes do medianu, tak delej compare and exchange mezi prvkama pole v danem smyslu razeni
				for ( shift = 0; shift < median; shift++) {
					CompareAndExchange(a, lowIndex + shift, lowIndex + median + shift, dir);
				}

			}
		}

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

