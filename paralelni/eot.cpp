	#include <stdio.h>
	#include <math.h>
	#include <omp.h>
	#include "../utils/2Darray.h"
	#include "eot.h"

	void swap(Array2D& a, int i1, int j1, int i2, int j2)
	{
		int val1 = a.getValueAt(i1, j1);
		int val2 = a.getValueAt(i2, j2);

		a.setValueAt(val2, i1, j1);
		a.setValueAt(val1, i2, j2);

	}

	//vrati true, pokud je poradi spravne
bool compare(int val1, int val2, int direction)
{
	bool retVal = false;

	if (direction == ASCENDING) {
		//porovnavame vzestupnost
		if (val1 <= val2) retVal = true;
	}

	if (direction == DESCENDING) {
		//porovnavame sestupnost
		if (val1 >= val2) retVal = true;
	}
	return retVal;
}


/*	int a[] - vstupni pole
 *	count - pocet prvku v radku
 */
void EOTRow(Array2D& a, int count, int rowNumber, int direction)
{
	int numOfRepeats = (int)ceil((double)count / 2);
	int i, chunk;

	chunk = CHUNKSIZE;
	int repeat1 = 2 * (count / 2) - 1; //pocet opakovani prvniho cyklu
	int repeat2 = 2 * (int)floor((double)((count - 1) / 2));

	//opakuj numOfRepeats x
	for (int j = 0; j < numOfRepeats; j++) {

		//paralelni blok s polem A a promennou chunk
		#pragma omp parallel num_threads(NUM_OF_THREAD) shared(a,count,chunk,repeat1,repeat2) private(i)
		{
			//licho-sude dvojce
		#pragma omp for schedule(dynamic,chunk)
			for (i = 0; i < repeat1; i += 2) {
				//compare and exchange
				int val1 = a.getValueAt(rowNumber, i);
				int val2 = a.getValueAt(rowNumber, i + 1);
				if (compare(val1, val2, direction) == false) { //pokud poradni neni spravne
					swap(a, rowNumber, i, rowNumber, i + 1);
				}
			}

			//sudo-liche dvojce
		#pragma omp for schedule(dynamic,chunk)
			for (i = 1; i < repeat2; i += 2) {
				//compare and exchange
				int val1 = a.getValueAt(rowNumber, i);
				int val2 = a.getValueAt(rowNumber, i + 1);
				if (compare(val1, val2, direction) == false) { //pokud poradni neni spravne
					swap(a, rowNumber, i, rowNumber, i + 1);
				}
			}
		} //end of paralel region
	}
}


void EOTColumn(Array2D& a, int count, int columnNumber, int direction)
{
	int numOfRepeats = (int)ceil((double)count / 2);
	int i, chunk;

	chunk = CHUNKSIZE;
	int repeat1 = 2 * (count / 2) - 1; //pocet opakovani prvniho cyklu
	int repeat2 = 2 * (int)floor((double)((count - 1) / 2));

	//opakuj numOfRepeats x
	for (int j = 0; j < numOfRepeats; j++) {

		//paralelni blok s polem A a promennou chunk
		#pragma omp parallel num_threads(NUM_OF_THREAD) shared(a,count,chunk,repeat1,repeat2) private(i)
		{
			//licho-sude dvojce
		#pragma omp for schedule(dynamic,chunk)
			for (i = 0; i < repeat1; i += 2) {
				//compare and exchange
				int val1 = a.getValueAt(i, columnNumber);
				int val2 = a.getValueAt(i + 1, columnNumber);
				if (compare(val1, val2, direction) == false) { //pokud poradni neni spravne
					swap(a, i, columnNumber, i + 1, columnNumber);
				}
			}

			//sudo-liche dvojce
		#pragma omp for schedule(dynamic,chunk)
			for (i = 1; i < repeat2; i += 2) {
				//compare and exchange
				int val1 = a.getValueAt(i, columnNumber);
				int val2 = a.getValueAt(i + 1, columnNumber);
				if (compare(val1, val2, direction) == false) { //pokud poradni neni spravne
					swap(a, i, columnNumber, i + 1, columnNumber);
				}
			}
		} //end of paralel region
	}
}


#ifdef DEBUG_EOT
int main(int argc, char** argv)
{

	Array2D a;

	a.setValueAt(2, 1, 0);
	a.setValueAt(4, 1, 1);
	a.setValueAt(10, 1, 2);
	a.setValueAt(5, 1, 3);
	a.setValueAt(8, 1, 4);
	a.setValueAt(20, 1, 5);
	a.setValueAt(22, 1, 6);
	a.setValueAt(32, 1, 7);
	a.setValueAt(42, 1, 8);
	a.setValueAt(-2, 1, 9);


	a.setValueAt(2, 1, 0);
	a.setValueAt(4, 2, 0);
	a.setValueAt(10, 3, 0);
	a.setValueAt(5, 4, 0);
	a.setValueAt(8, 5, 0);
	a.setValueAt(20, 6, 0);
	a.setValueAt(22, 7, 0);
	a.setValueAt(32, 8, 0);
	a.setValueAt(42, 9, 0);
	a.setValueAt(-2, 0, 0);


	EOTRow(a, 10, 1, 1);
	//EOTColumn(a, 10, 0, DESCENDING);

	a.print();

	return 0;
}
#endif

