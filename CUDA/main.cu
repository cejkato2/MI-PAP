#include <stdio.h>
#include <stdlib.h>
#include "eot.cuh" //eot sort in cuda
#include "constant.h"

void testSorted(int a[],int size)
{
	bool sorted = true;
	printf("Testuji serazenou posloupnost --->\n");
	for(int i=1;i<size;i++){
		if(a[i-1] > a[i]){
			sorted = false;
			break;
		}
	}
	if(sorted){
		printf("Serazeno\n");
	}else{
		printf("Neserazeno --->\n");
	}
}

void generateArray(int a[],int size)
{
	srand(time(NULL));
	for(int i=0;i<size;i++){
	  a[i]=(rand() % MODULE);
	}
}


void printArray(int a[],int size)
{
	for(int i=0;i<size;i++){
		printf("%d ",a[i]);
	}
	printf("\n");
}

int main(int argc, char** argv)
{
int* a = new int[ARRAY_SIZE];

generateArray(a, ARRAY_SIZE);

#ifdef DEBUG_GLOBAL
printArray(a,ARRAY_SIZE);
#endif

oddeven(a,ARRAY_SIZE);
testSorted(a,ARRAY_SIZE);

#ifdef DEBUG_GLOBAL
printArray(a,ARRAY_SIZE);
#endif

return 0;
}
