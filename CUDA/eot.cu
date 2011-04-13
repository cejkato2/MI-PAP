#include <cuda.h>
#include <driver_types.h>
#include <cuda_runtime_api.h>
#include "eot.cuh"
#include "utils/utils.cuh"
#include "constant.h"
#include <math.h>
#include <utils/cuda_syn_block.cu> //synchronizace napric barierama -- prasarna vkladat primo ceckovy kod, ale :
//You must define __device__ functions within the compilation unit they are called in, and their behavior similar to functions declared with the c++ inline keyword.

// compare and swap; copies from the f to t, swapping f[i] and
// f[j] if the higher-index value is smaller; it is required that i < j
/*
local_f - lokalni pole odkud se berou data
local_t - lokalni pole, kam se docasne kopiruje
global_h - globalni pole pro transpozice
i,j - indexy
n - pocet prvku
my_global_pos - globalni pozice jadra
me - moje cislo jadra
phase - aktualni faze sudo licha nebo licho suda
----------------------------------------------------
POZOR: DATA V TETO FAZI MUSI BYT JAK V GLOBALNI TAK V LOKALNI CASTI KOHERENTNI!!
*/
__device__ void cas(int *local_f, int *local_t, int* global_h, int i, int j, int n, int my_global_pos, int me, int phase)
{
//1) Index I mimo blok AND moje globalni_pozice = krajni prvky N
	if(my_global_pos == 0 && i < 0){return ;} //kontrola leve zarazky
	if(my_global_pos == (n-1) && j>=n){return ;} //kontrola prave zarazky
//2) Jsme v ramci globalniho pole N, muzem zacit provadet vymeny - jsme v ramci bloku mimo krajni prvky? -> pokud ano, trid ve sdilene pameti
	if( ((i>0 && i< (NUM_OF_THREADS-1)) && (phase == LS)) //pokud nejsi mimo v LS fazi, tak trid v ramci lokalniho pole
	    ((i==(NUM_OF_THREADS-1)
		   || phase == SL){ //pokud mas SL fazi, tak je vse OK a muzes vse tridit v ramci lokalniho pole
		if (me == i) { //v teto casti jsme v ramci indexu sdileneho pole 
			if (local_f[i] > local_f[j]) local_t[me] = local_f[j];
			//else local_t[me] = local_f[i];
		} else { // me == j
			if (local_f[i] > local_f[j]) local_t[me] = local_f[i];
			//else local_t[me] = local_f[j];
		}			
	}else{ //jinak musis komunikovat do globalni pameti, protoze jsi vlakno s krajnim indexem a 
	
	}
}

// does one iteration of the sort
/*
da - ukazatel od prvniho pole
daaux - ukazatel do temp pole
n - velikost trideneho pole
iter - aktualni iterace
barnos - pomocne pole pro synchro mezi bloky
*/
__global__ void oekern(int *h_da, int n,volatile unsigned int* barnos)
{
	int tix=threadIdx.x;
	int d_index=blockIdx.x*NUM_OF_THREADS + tix; //globalni index v poli v hlavni pameti
//1) Kazde vlakno nakopiruje do lokalni pameti bloku sve data
	__shared__ int sData[NUM_OF_THREADS]; //alokace lokalni pameti
	__shared__ int sData_aux[NUM_OF_THREADS]; //temp datove pole
	sData[tix] = h_da[tix]; //prekopiruji si data do lokalni pameti

//2) Pockame, az to udelaji vsichni ve vsech blocich
	__syncblocks(barnos); 

//3) N-krat budeme opakovat transpozice nad svou casti dat
//Pozn. : SL liche jsou v ramci sdilene pameti. U LS musi krajni vlakna komunikovat prez globalni pamet.
	unsigned int iter;
	for(iter=0; iter < n; iter++){ 

		if( (iter%2) == 1){
		//provadej LS vymenu
			if( (tix%2) == 0){
				cas(sData, sData_aux, h_da, tix, tix+1, n, d_index, tix, iter);
			}else{
				cas(sData, sData_aux, h_da, tix-1, tix, n, d_index, tix, iter);
			}
		}else{
		//provadej SL vymenu => zacina se zde v prvni iteraci
			if( (tix%2) == 1){
				cas(sData, sData_aux, h_da, tix,tix+1, n, d_index, tix, iter);
			}else{
				cas(sData, sData_aux, h_da, tix-1, tix, n, d_index, tix, iter);
			}
		}
	}
}


void oddeven(int *ha, int n)
{
	int *da;
	int dasize = n * sizeof(int);

	HANDLE_ERROR(cudaMalloc((void **)&da, dasize));
	HANDLE_ERROR(cudaMemcpy(da, ha, dasize, cudaMemcpyHostToDevice));
	// the array daaux will serve as "scratch space"
	int *daaux;
	HANDLE_ERROR(cudaMalloc((void **)&daaux, dasize));

	int numOfBlocks = (int) ceil(ARRAY_SIZE/NUM_OF_THREADS); //number of blocks
	
	// ===== alokuj pole pro synchro =====
	unsigned int* h_barnos = new unsigned int[numOfBlocks]; //pomocne pole
	//inicializuj pole cislem 0
	for(int i=0;i<numOfBlocks;i++)	h_barnos[i] = 0;
	
	volatile unsigned int* barnos; //nase promenna, kterou budem ladovat cudu
	HANDLE_ERROR(cudaMalloc((void **) &barnos, sizeof(int)*numOfBlocks)); //alokujem si na to mistecko
	HANDLE_ERROR(cudaMemcpy((void*)barnos,h_barnos, sizeof(int)*numOfBlocks,cudaMemcpyHostToDevice)); //naládujem pole
	
	delete[] h_barnos; //a uklidime po sobe	pole,co nepotrebujeme				

	// ===== priprav sturkturu pro deleni problemu =====
	dim3 dimGrid(numOfBlocks, 1);
	dim3 dimBlock(NUM_OF_THREADS, 1, 1);

	// ===== deme na problem =====	
	oekern <<< dimGrid, dimBlock >>> (da, n, barnos); //eot sort v radku
	
	//free malocs
	HANDLE_ERROR(cudaFree(da));
	HANDLE_ERROR(cudaFree(daaux));
}

