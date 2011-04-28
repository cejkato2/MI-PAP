#include <stdio.h>
#include <stdlib.h>
#include "eot.cuh" //eot sort in cuda
#include "constant.h"
#include <math.h>

//jednorozmerne pole se bere jako 2D pole
void testSorted(int a[],int size)
{
	bool sorted = true;
	printf("Testuji serazenou posloupnost ---> ");
	for(int i=1;i<size/2;i++){
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
	printf("Generuji testovaci data .. (%d)\n",size);
	srand(time(NULL));
	for(int i=0;i<size;i++){
	  a[i]=(rand() % MODULE);
	}
}


void printArray(int a[],int row,int col)
{
	int index;
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			index=i*col + j;		
			printf("%d ",a[index]);
		}
	printf("\n");
	}
	printf("\n");
}

int main(int argc, char** argv)
{

int ARRAY_SIZE = 16;

for(int tmp=4;tmp<26;ARRAY_SIZE*=2,tmp++){

int* a = new int[ARRAY_SIZE];

generateArray(a, ARRAY_SIZE);

//pocet radku a sloupcu problemu
int col;
int row; 


if(ARRAY_SIZE <= NUM_OF_THREADS){
	//nema cenu delit problem
	row=1;
	col=ARRAY_SIZE;
}else{
	//rozdelime problem 
	//int blkCount =  (int) ceil(ARRAY_SIZE/NUM_OF_THREADS);
	//najdem co mozny nejvetsi pocet radku
	//row = sqrt(blkCount);
	//while((blkCount%row)!=0){
	//	row--;
	//}		
	
	//col=(blkCount/row)*NUM_OF_THREADS;	
	row=NUM_OF_THREADS/4;
	col=ARRAY_SIZE/row;
}

printf("Matice --> %d radku, %d sloupcu.\n\n",row,col);

#ifdef DEBUG_GLOBAL
printArray(a,row,col);
#endif

ShearOddeven(a,row,col);

#ifdef DEBUG_GLOBAL
printArray(a,row,col);
#endif
}
return 0;
}
