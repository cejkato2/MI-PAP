#include <math.h>
#include <iostream>
#include <omp.h>
#include "../utils/array.h"
#include "../utils/2Darray.h"
#include "../utils/loader.h"
#include "../utils/sortTester.h"

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

void BitonicMergeSort(int a[], int low, int size, int numOfThreads)
{

	int stride;     //stride, kterym to budu delit (rika kolik toho budu slucovat) - zacina se na 2
	int tmpStride;  //temporatery stride

//dokud neslucuji stride o velikosti celeho pole -zvetsovani o dvojnasobek (shift vlevo)
	for (stride = 2; stride <= size; stride *= 2) {

		//stride - aktualni zpracovavane velikosti bloku, tedka zpracovavat dokud nedostaneme velikost 1 (pocet bitonickych splitu)
		//vypocteme aktualni smer zareni
		int numOfArrows = size / stride;        //pocet hlavnich "sipek" s aktualnim stridem

		// zacatek privatnich promennych (pro kazde vlakno vlastni)
		int actualArrow;                        //aktualni zvolena hlavni sipka
		int dir;                                //aktualni smer zarezni pro hlavni sipku
		int arrowBase;                          //zakladna aktualni volene sipky
		int actualSubiter;                      //aktualni subiterace sipky
		int numOfSubiters;                      //pocet subiteraci
		int lowIndex;                           //spodni intex subiterace
		int shift;                              //posunuti v subiteraci
		int median;                             //median pole pro bitonicky split

		#pragma omp parallel shared(a,numOfArrows,stride) private(dir,tmpStride,arrowBase,actualArrow,numOfSubiters,actualSubiter,lowIndex, \
		shift,median) num_threads(numOfThreads)
		{

		#pragma omp for schedule(static)
			for (actualArrow = 0; actualArrow < numOfArrows; actualArrow++) {
				//tedka mam vybranou hlavni sipku, urcim jeji smer razeni
				if ( (actualArrow % 2) == 1) {
					dir = DESCENDING;       //licha sipka -> sestupne
				}else{
					dir = ASCENDING;        //suda sipka -> vzestupne
				}

				//tedka musim rozkldat velkou sipku na sipky mensi to delame nez je stride roven 1
				tmpStride =  stride;                    //zaciname s velkou sipkou
				arrowBase = actualArrow * stride;       //zacatek velke sipky

				//v kazde iteraci se musi upravit velikost stridu
				for (; tmpStride > 1; tmpStride /= 2) {
					numOfSubiters = stride / tmpStride;                             //pocet subiteraci v aktualnim zmensenem stridu
					for (actualSubiter = 0; actualSubiter < numOfSubiters; actualSubiter++) {
						lowIndex = arrowBase + actualSubiter * tmpStride;       //zacatek sipky subiterace
						median = tmpStride / 2;                                 //median pole kam delat bitonic split
						//bitonic split
						for (shift = 0; shift < median; shift++) {
							CompareAndExchange(a, lowIndex + shift, lowIndex + median + shift, dir);
						}
					}
				}

			}
		}
	}
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

	BitonicMergeSort(a.getData(), 0, a.getSize(), THREADS);

	std::cout << "Elapsed time: " << omp_get_wtime() - timer << "s" << std::endl;

	testSortedArray(a, 1);

	return 0;
}

