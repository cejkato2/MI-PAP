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

//dokud neslucuji stride o velikosti celeho pole -zvetsovani o dvojnasobek (shift vlevo)
	for (stride = 2; stride <= size; stride *= 2) {

		//stride - aktualni zpracovavane velikosti bloku, tedka zpracovavat dokud nedostaneme velikost 1 (pocet bitonickych splitu)
		//vypocteme aktualni smer zareni
		int numOfArrows = size / stride; //pocet hlavnich "sipek" s aktualnim stridem
		int actualArrow; //aktualni zvolena hlavni sipka		
		int dir;	//aktualni smer zarezni pro hlavni sipku
	
		for(actualArrow=0 ; actualArrow < numOfArrows; actualArrow++){
			//tedka mam vybranou hlavni sipku, urcim jeji smer razeni
			if( (actualArrow%2) == 1){
				dir = DESCENDING; //licha sipka -> sestupne
			}else{
				dir = ASCENDING; //suda sipka -> vzestupne
			} 
		
			//tedka musim rozkldat velkou sipku na sipky mensi to delame nez je stride roven 1
			tmpStride =  stride; //zaciname s velkou sipkou
			int arrowBase = actualArrow*stride; //zacatek velke sipky
		
		while(tmpStride > 1){
			int numOfSubiters = stride / tmpStride; //pocet subiteraci v aktualnim zmensenem stridu
			for(int actualSubiter=0; actualSubiter < numOfSubiters; actualSubiter++){
				int lowIndex = arrowBase + actualSubiter*tmpStride; //zacatek sipky subiterace
				int median = tmpStride / 2; //median pole kam delat bitonic split
				//bitonic split
				for(int shift=0; shift < median; shift++){
					CompareAndExchange(a,lowIndex+shift,lowIndex+median+shift,dir);
				}
			} 	
			//upravim velikost stridu -> na pulku (poustim to na mensi casti)
			tmpStride = tmpStride / 2;
			
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

