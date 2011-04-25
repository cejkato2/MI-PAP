#ifndef CUDA_COMPARER_CU_
#define CUDA_COMPARER_CU_


/*kernel pro porovnavani smeru.
Vraci vysledek v res, vraci true => je to spravne jinak false.
a - levy prvek
b - pravy prvek
*/
__device__ void compare_k(int a, int b, int dir, bool* res)
{
	if(dir == ASCENDIG){
		//vzestupny smer
		if( a <= b){
			*res = true; //poradi je spravne
		}else{
			*res = false; //poradi neni spravne
		} 
	}else{
		//sestupny smer
		if(a >= b){
			*res = true; //poradi je spravne
		}else{
			*res = false; //poradi neni spravne
		}
	}
}

#endif
