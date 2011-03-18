#include "../array.h"
#include "sortTester.h"
#include <iostream>
#include <omp.h>

void testSortedArray(Array& a,int numOfThreads)
{

int* b = a.getData();
int size = a.getSize();

int i;
int ok;

std::cout<<"Zacinam kontrolovat pole -->"<<std::endl;

#pragma omp parallel shared(b,ok,size) private(i) num_threads(numOfThreads)
{
	#pragma omp master
	ok=1;
	
 	#pragma for schedule(static)
 	for (i=1; i < size; i++){
		if(b[i-1] > b[i]){
			ok = 0;
		}
	}

	#pragma omp master
	if(ok == 1){
		std::cout<<"Pole je zesortovane"<<std::endl;
	}else{
		std::cout<<"Pole neni zesortovane"<<std::endl;
	}

}

}

