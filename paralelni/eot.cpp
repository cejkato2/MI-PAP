#include <stdio.h>
#include <math.h>
#include <omp.h>

#define CHUNKSIZE 100
#define NUM_OF_THREAD 5

void printArray(int a[], int count)
{
	for (int i = 0; i < count; i++) {
		printf("%i ", a[i]);
	}
	printf("\n");
}


/*	int a[] - vstupni pole
 *	count - pocet prvku v poli
 */
void EOT(int a[], int count)
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
				if (a[i] > a[i + 1]) {
					int tmp = a[i];
					a[i] = a[i + 1];
					a[i + 1] = tmp;
				}
			}

			//sudo-liche dvojce
		#pragma omp for schedule(dynamic,chunk)
			for (i = 1; i < repeat2; i += 2) {
				//compare and exchange
				if (a[i] > a[i + 1]) {
					int tmp = a[i];
					a[i] = a[i + 1];
					a[i + 1] = tmp;
				}
			}
		} //end of paralel region
	}
}

int main(int argc, char** argv)
{

	int a[12] = { 4, 2, 1, 9, 3, 2, 4, 5, -1, 2, 3, 4 };

	printArray(a, 12);

	EOT(a, 12);

	printArray(a, 12);
	return 0;
}
