#include <cuda.h>
#include <driver_types.h>
#include <cuda_runtime_api.h>
#include "eot.cuh"
#include "utils/utils.cuh"
#include "constant.h"
#include <math.h>
#include "utils/cuda_comparer.cu" //komparator pro cudu


// compare and swap; copies from the f to t, swapping f[i] and
// f[j] if the higher-index value is smaller; it is required that i < j
/*
local_f - lokalni pole odkud se berou data
local_t - lokalni pole, kam se docasne kopiruje
global_h - globalni pole pro transpozice
i,j - indexy
n - pocet prvku
my_global_pos - globalni pozice jadra
me - moje X pozice jako threadu
phase - aktualni faze sudo licha nebo licho suda
dir - smysl trideni
my_global_x - globalni x pozice
localId - id v lokanlni pameti
col_size - pocet sloupecku
blkXDim - sirka bloku v X 
----------------------------------------------------
POZOR: DATA V TETO FAZI MUSI BYT JAK V GLOBALNI TAK V LOKALNI CASTI KOHERENTNI!! A pracuje jenom s X koordinatama vlakna v gridu
*/
__device__ void cas_row(int *local_f, int *local_t, int* global_h, 
			int i, int j, int n, int me, int phase,int dir, 
			int my_global_x, int my_global_pos, int localId,int col_size,int blkXDim)
{
//1) Index I mimo blok AND moje globalni_pozice = krajni prvky N
	if((my_global_x == 0) && i < 0){  //kontrola leve zarazky - jsi na krajni pozici a presahujes blok?
		return ;
	} 

	if((my_global_x == (col_size-1)) && j>=blkXDim){ //kontrola prave zarazky -> jsi na globalni pozici a presahujes blok?
		return ;
	}
	int ii;
	int jj; //promenne pro prepocet na faze v lokalni pameti ==> nutne 
	register bool res; //vysledek komparace
//2) Jsme v ramci globalniho pole N, muzem zacit provadet vymeny - jsme v ramci bloku mimo krajni prvky? -> pokud ano, trid ve sdilene pameti
	if( ((i>0 && i<(blkXDim-1)) && (phase == LS)) //pokud nejsi mimo v LS fazi, tak trid v ramci lokalniho pole
		   || phase == SL){ //pokud mas SL fazi, tak je vse OK a muzes vse tridit v ramci lokalniho pole
		if (me == i) { //v teto casti jsme v ramci indexu sdileneho pole
			ii=localId;
			jj=localId+1; 
			compare_k(local_f[ii], local_f[jj], dir, &res);	
			if (res == false) local_t[ii] = local_f[jj];
			else local_t[ii] = local_f[ii];
		} else { // me == j
			ii=localId-1;
			jj=localId;
			compare_k(local_f[ii], local_f[jj], dir, &res);	
			if (res == false) local_t[jj] = local_f[ii];
			else local_t[jj] = local_f[jj];
		}			
	}else{ //jinak musis komunikovat do globalni pameti, protoze jsi vlakno s krajnim indexem a 
		if(me == (blkXDim - 1)){ //pokud je me cislo rovno  krajnimu cislu vlakna (tj NUM_OF_THREADS -1) -> jsem i
			ii=localId;
			compare_k(local_f[ii], global_h[my_global_pos+1], dir, &res);
			if(res == false) local_t[ii] = global_h[my_global_pos+1]; //pokud jsem vetsi jak muj glob. soused, tak si upravim v lokalni pameti data
			else local_t[ii] = local_f[ii]; //jinak jsem na to spravne a prekopiruju si to do tmp pole
		}else{//jsem j
			jj=localId;
			compare_k(global_h[my_global_pos-1], local_f[jj], dir, &res);
			if(res == false) local_t[jj] = global_h[my_global_pos-1]; //pokud je muj globalni soused vetsi jak ja, tak si uravim v lokalni pameti
			else local_t[jj] = local_f[jj]; //jinak jsem na tom dobre a nic menit nemusim
		}	
	}
}


/*
local_f,local_t - ostre a tmp pole
i,j - indexy prvku na porovnani
n - pocet prvku
meId - moje lokalni I lokalni ID
blkXDim - dimenze bloku
*/
//POZOR!! Indexy i a j jsou efektivni ukazatele do lokalni pameti
__device__ void cas_col(int *local_f, int *local_t, int i, int j, int max_index, int meId)
{
	if(i < 0 || j>=max_index){ //kontrola zarazek
		return;
	}

	if(meId == i){ 
	//me==i
		if(local_f[i] > local_f[j]){
			local_t[meId]=local_f[j];	
		}else{
			local_t[meId]=local_f[i];
		}
	}else{
	//me==j
		if(local_f[i] > local_f[j]){
			local_t[meId]=local_f[i];
		}else{
			local_t[meId]=local_f[j];
		}
	}
}

/*
da - ukazatel od prvniho pole
daaux - ukazatel do temp pole
n - velikost trideneho pole
iter - aktualni iterace
barnos - pomocne pole pro synchro mezi bloky
blkDimX - dimense X bloku
*/
__global__ void ShearOekern(int *h_da,int* h_dax, int row_size, int col_size,int blkDimX,int act_iter,int iter, int sh_iter)
{
//vypocet souradnice X a Y
	int tix=threadIdx.x;
	int tiy=threadIdx.y;
	int localId=tiy*blkDimX + tix;
	
	//globalni index do pole	
	int d_index=tiy*col_size + blkDimX*blockIdx.x+tix; //globalni index v poli v hlavni pameti
	
	//souradnice x,y globalniho pole
	int x = blkDimX*blockIdx.x+tix;
	int y = tiy;

	//1) Kazde vlakno nakopiruje do lokalni pameti bloku sve data
	__shared__ int sData[NUM_OF_THREADS]; //alokace lokalni pameti
	__shared__ int sData_aux[NUM_OF_THREADS]; //temp datove pole
	sData[localId] = h_da[d_index]; //prekopiruji si data do lokalni pameti
	sData_aux[localId] = sData[localId];
	////////// DEBUG ///////////
//	h_da[d_index] = localId;	
//	h_da[d_index] = x;
//	h_da[d_index] = y;

	__syncthreads();
	
	unsigned int phase; //SL nebo LS faze porovnani
	int dir; //smer razeni

		//urci fazi -> podle tveho radku
		if((iter%2) == 0){
			phase = SL;
		}else{
			phase = LS;
		}
	
		//urci smer razeni
		if(sh_iter == SH_ROW){
			if((y%2) == 0){
				dir=ASCENDIG;
			}else{
				dir=DESCENDING;
			}
		}else{
			dir=ASCENDIG;
		}
					
			
		if(sh_iter == SH_ROW){
			if(phase == SL){
			//provadej SL vymenu
				if( (tix%2) == 0){
					cas_row(sData, sData_aux, h_da, tix, tix+1, col_size, tix, phase, dir, x, d_index,localId, col_size, blkDimX);
				}else{
					cas_row(sData, sData_aux, h_da, tix-1, tix, col_size, tix, phase, dir, x, d_index,localId, col_size, blkDimX);
				}
			}else{
			//provadej LS vymenu => zacina se zde v prvni iteraci
				if( (tix%2) == 1){
					cas_row(sData, sData_aux, h_da, tix,tix+1, col_size, tix, phase, dir, x, d_index,localId, col_size, blkDimX);
				}else{
					cas_row(sData, sData_aux, h_da, tix-1, tix, col_size, tix, phase, dir, x, d_index,localId, col_size, blkDimX);
				}
			}
		}else{
		//SH_COLUMN faze
			if(phase == SL){
				if((tiy%2) == 0){
					cas_col(sData, sData_aux, localId, localId+blkDimX, NUM_OF_THREADS, localId);	
				}else{
					cas_col(sData, sData_aux, localId-blkDimX, localId, NUM_OF_THREADS, localId);
				}
			}else{
				if((tiy%2) == 1){
					cas_col(sData, sData_aux, localId, localId+blkDimX, NUM_OF_THREADS, localId);
				}else{
					cas_col(sData, sData_aux, localId-blkDimX, localId, NUM_OF_THREADS, localId);
				}
			}
		}
	
	__syncthreads();
	h_dax[d_index] = sData_aux[localId];
}


void ShearOddeven(int *ha, int row_count,int col_count)
{
	int *da,*dax;
	int n = row_count*col_count;
	int dasize = n * sizeof(int);
	//mereni casu
	cudaEvent_t start, stop;
	float elapsedTime;
	
	HANDLE_ERROR( cudaEventCreate( &start ) );
	HANDLE_ERROR( cudaEventCreate( &stop ) );


	//choose best device
	int num_devices, device;
	cudaGetDeviceCount(&num_devices);
	if (num_devices > 1) {
      	int max_multiprocessors = 0, max_device = 0;
      		for (device = 0; device < num_devices; device++) {
              		cudaDeviceProp properties;
              		cudaGetDeviceProperties(&properties, device);
              		
			if (max_multiprocessors < properties.multiProcessorCount) {
                      		max_multiprocessors = properties.multiProcessorCount;
                	     	max_device = device;
        	      	}
      		}
	
      		cudaSetDevice(max_device);
		
		//vystup vlastnosti
//		printf("Device number --> %d\n", num_devices);
//		printf("Multiprocesor count --> %d\n", max_multiprocessors);
//		printf("========================================\n");
//		printf("Vybrane zarizeni --> %d\n", device);
	}
//	printf("Pocet threadu --> %d\n\n",NUM_OF_THREADS);

	HANDLE_ERROR(cudaMalloc((void **)&da, dasize));
	HANDLE_ERROR(cudaMalloc((void **)&dax, dasize));
	HANDLE_ERROR(cudaMemcpy(da, ha, dasize, cudaMemcpyHostToDevice));
	
	int xBlkDim = NUM_OF_THREADS/row_count; //x rozmer bloku
	int yBlkDim = row_count;
	int xMatrix = col_count; //x rozmer matice	
	int numOfBlocks = (int) ceil(xMatrix/xBlkDim); //pocet bloku
	
	// ===== priprav sturkturu pro deleni problemu =====
	dim3 dimGrid(numOfBlocks, 1); //pustime to na pocet bloku (jak jsme spocitali)
	dim3 dimBlock(xBlkDim,yBlkDim, 1); //a kazdy blok bude mit rozmery

//	printf("Pocet bloku-->%d\n",numOfBlocks);
//	printf("Rozmery bloku--> %d radku, %d sloupcu\n",yBlkDim,xBlkDim);
	// ===== deme na problem =====	
	int numOfIter;	
	int sh_iter;
	int numOfPhases = 2 * ((int)floor(log2((float)row_count))) + 1; //pocet iteraci shearsortu
	//int numOfPhases=1;
	HANDLE_ERROR( cudaEventRecord( start, 0 ) );
	
	for(int act_iter=0; act_iter < numOfPhases; act_iter++){
		
			if((act_iter%2)==0){
				sh_iter=SH_ROW;
			}else{
				sh_iter=SH_COL;
			}
		
			//vyber poctu iteraci
			if(sh_iter == SH_ROW){
				numOfIter=col_count;
			}else{
				numOfIter=row_count;
			}	

//		printf("Pocet iteraci %d\n",numOfIter);	
		for(int iter=0; iter < numOfIter ; iter++){
			ShearOekern <<< dimGrid, dimBlock >>> (da,dax,row_count,col_count,xBlkDim,act_iter,iter,sh_iter);
			cudaThreadSynchronize();
			HANDLE_ERROR(cudaMemcpy(da,dax,dasize,cudaMemcpyDeviceToDevice));
			}
	}
	HANDLE_ERROR(cudaEventRecord( stop, 0 ) );
	HANDLE_ERROR(cudaMemcpy(ha,da,dasize,cudaMemcpyDeviceToHost));
	

	//zjisteni casu
	HANDLE_ERROR( cudaEventSynchronize( stop ) );
	HANDLE_ERROR( cudaEventElapsedTime( &elapsedTime, start, stop ) );
	printf("\n\n-----------------------------------------------------\n");
	printf( "GPU čas: %g ms ---> prvku %d\n", elapsedTime,row_count*col_count );
	printf("-----------------------------------------------------\n\n");

	//uklizeni
	HANDLE_ERROR( cudaEventDestroy( start ) );
	HANDLE_ERROR( cudaEventDestroy( stop ) );

	//free malocs
	HANDLE_ERROR(cudaFree(da));
	HANDLE_ERROR(cudaFree(dax));
}

