#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include "shearmergesort.h"
#include "../utils/2Darray.h"

/**
 * Komparace podle smyslu razeni.
 * vraci true, pokud je splnen smysl razeni, jinak vrati false.
 * Val1 - leva hodnota v poli, val2 - prava hodnota v poli
 */
bool compare(int val1, int val2, int dir)
{
	if (dir == ASCENDING) {
//vzestupny smer
		if (val1 <= val2) {
			return true;
		}else{
			return false;
		}
	}else{
//sestupny smer
		if (val1 >= val2) {
			return true;
		}else{
			return false;
		}
	}

}

/**
* Podle metody pristupu vratime prvek z daneho pole
*/
int getArrayValue (Array2D& a,int fixPos,int iter,int arrAcc)
{
	int retVal = 0;
	//radkovy pristup - zafixujeme radek, menime sloupec
	if(arrAcc == ROW){
		retVal = a.getValueAt(fixPos,iter);
	}

	//sloupcovy pristup - zafixujeme sloupec
	if(arrAcc == COLUMN){
		retVal = a.getValueAt(iter,fixPos);
	}
return retVal;
}

//to same co predtim, ale operace ukladani
void setArrayValue (Array2D& a,int fixPos,int iter,int val,int arrAcc)
{
	//radkovy pristup - zafixujeme radek, menime sloupec
	if(arrAcc == ROW){
		a.setValueAt(val,fixPos,iter);
	}

	//sloupcovy pristup - zafixujeme sloupec
	if(arrAcc == COLUMN){
		 a.setValueAt(val,iter,fixPos);
	}
}

/**
 * Slevani pro Merge sort
 * @param array pole k serazeni
 * @param aux pomocne pole (stejne velikosti jako razene)
 * @param left prvni index, na ktery smim sahnout
 * @param right posledni index, na ktery smim sahnout
 */
void merge(Array2D& a, int aux[], int left, int right,int fixedPoint, int arrayAcces, int dir)
{
	int middleIndex = (left + right) / 2;
	int leftIndex = left;
	int rightIndex = middleIndex + 1;
	int auxIndex = left;

	while (leftIndex <= middleIndex && rightIndex <= right) {
		if ( compare(	getArrayValue(a,fixedPoint,leftIndex,arrayAcces), //data
				getArrayValue(a,fixedPoint,rightIndex,arrayAcces),//data
				dir)) {						  //smer razeni
			aux[auxIndex] = getArrayValue(a,fixedPoint,leftIndex++,arrayAcces);
		}else{
			aux[auxIndex] = getArrayValue(a,fixedPoint,rightIndex++,arrayAcces);
		}
		auxIndex++;
	}
	while (leftIndex <= middleIndex) {
		aux[auxIndex] = getArrayValue(a,fixedPoint,leftIndex++,arrayAcces);
		auxIndex++;
	}
	while (rightIndex <= right) {
		aux[auxIndex] = getArrayValue(a,fixedPoint,rightIndex++,arrayAcces);
		auxIndex++;
	}

	for (int i = left; i <= right; i++) {
		setArrayValue(a,fixedPoint,i,aux[i],arrayAcces);
		
	}
}

/** Razeni slevanim (od nejvyssiho)
 * @param array pole k serazeni
 * @param aux pomocne pole stejne delky jako array
 * @param left prvni index na ktery se smi sahnout
 * @param right posledni index, na ktery se smi sahnout
 * @param fixedPoint - fixnuty bod v pristupu k datum(muzem zafixovat radek nebo sloupec)
 * @param dir smer razeni
 */
void mergeSort(Array2D& a, int aux[], int left, int right, int fixedPoint, int arrayAcces,int dir)
{
	if (left == right) return;
	int middleIndex = (left + right) / 2;
	mergeSort(a, aux, left, middleIndex, fixedPoint, arrayAcces, dir);
	mergeSort(a, aux, middleIndex + 1, right, fixedPoint, arrayAcces, dir);
	merge(a, aux, left, right, fixedPoint, arrayAcces,dir);

}



#ifdef DEBUG_MERGESORT

void generateTable(Array2D& a, int x, int y)
	{
	for (int i = 0; i < x; i++) {
		for (int j = 0; j < y; j++) {
			int num = (rand() % 100) + 1;
			a.setValueAt(num, i, j);
		}
	}
}

int main(int argc, char** argv)
{
	Array2D a(4,4);
	int* aux = new int[4];
	
	generateTable(a,4,4);
	
	printf("Vstupni pole\n:");
	a.print();
	printf("\n\n");
	
	mergeSort(a, aux, 0, 3, 1, ROW, ASCENDING);

	printf("Vytupni pole\n:");
	a.print();
	printf("\n\n");

	printf("\n");

	return 0;
}
#endif

