#ifndef EOT_H

//nastaveni parametru
#define CHUNKSIZE 100
#define NUM_OF_THREAD 5

//konstanty
#define ASCENDING 0
#define DESCENDING 1


void EOTColumn(Array2D& a, int count,int columnNumber,int direction);
void EOTRow(Array2D& a, int count,int rowNumber,int direction);

#endif
