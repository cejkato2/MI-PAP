#ifndef CUDA_SYN_BLOCK_CU_
#define CUDA_SYN_BLOCK_CU_

#include <cuda.h>

/*http://aggregate.org/MAGIC/#GPU SyncBlocks*/

inline __device__ void __syncblocks(volatile unsigned int *barnos) 
{ 

/* First, sync within each Block */
__syncthreads();
/* Pick a representative from each (here, 1D) block */
if (threadIdx.x == 0) {
  /* Get my barrier number */
  int barno = barnos[blockIdx.x] + 1;
  int hisbarno;
  int who = (blockIdx.x + 1) % gridDim.x;
  /* Check in at barrier */
  barnos[blockIdx.x] = barno;
  /* Scan for all here or somebody passed */
  do {
    /* Wait for who */
    do {
      hisbarno = barnos[who];
    } while (hisbarno < barno);
    /* Bump to next who */
    if (++who >= gridDim.x) who = 0;
  } while ((hisbarno == barno) && (who != blockIdx.x));
  /* Tell others we are all here */
  barnos[blockIdx.x] = barno + 1;
}
/* Rejoin with rest of my Block */
__syncthreads();

} 

#endif
