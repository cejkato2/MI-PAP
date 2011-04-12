#include <stdio.h>
#include <stdlib.h>
#include "eot.cuh" //eot sort in cuda
#include "constant.h"

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
printArray(a,ARRAY_SIZE);

oddeven(a,ARRAY_SIZE);

printArray(a,ARRAY_SIZE);
return 0;
}
