#ifndef LOADER_H
#include <math.h>
#define ARRAY_SIZE_EXP 10
#define ARRAY_SIZE ((int) pow(2,ARRAY_SIZE_EXP))

#ifndef THREADS
  #define THREADS 2
#endif

void getSettings(char **filename, int *amount, int *threads, int argc, char **argv);
void getFilename(char **filename, int argc, char **argv);
void loadValues(char *filename, Array &array, int amount = 0);
void loadValues(char *filename, Array2D *a, int amount = 0);

#endif
