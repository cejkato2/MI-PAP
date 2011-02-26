#include <iostream>
#include <fstream>
#include "mergesort.h"
#include <getopt.h>
#include <string.h>



int main(int argc, char **argv)
{
  char *filename = NULL;
  static struct option long_options[] =
  {
    /* These options set a flag. */
    {"file",    required_argument, 0, 'f'},
    {0, 0, 0, 0}
  };
  /* getopt_long stores the option index here. */
  int option_index = 0;
  int c;

  while (1) {
    c = getopt_long (argc, argv, "f:",
        long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c) {
      case 0:
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0)
          break;
        std::cout << "option " << long_options[option_index].name << std::endl;
        if (optarg)
          std::cout << " with arg " << optarg;
        std::cout << std::endl;
        break;

      case 'f':
        filename = optarg;
        break;
      default:
        return(1);
    }
  }

  if (filename == NULL) {
    std::cerr << "No input file specified\n" << std::endl;
    return(1);
  }
  
  std::ifstream in;
  in.open(filename);
  if (in.good() != true) {
    std::cerr << "File not found" << std::endl;
    return(1);
  }

  int cap = 10;
  int size = 0;

  int *a = new int[cap];
  while (in.eof() != true) {
    if (size == cap+1) {
      int *temp = new int[cap*2];
      for (int i=0; i<size; ++i) {
        temp[i] = a[i];
      }
      cap = cap*2;
      delete [] a;
      a = temp;
    }

    in >> a[size++];
    if (in.eof() == true) {
      size--;
    }
  }
  
//  std::cout << "Loaded data:" << std::endl;
//  for (int i=0; i<size; ++i) {
//    std::cout << a[i] << std::endl;
//  }

  int *aux = new int[size];

  mergeSort(a, aux, 0, size-1);

//  std::cout << "Sorted: " << std::endl;

  for (int i = 0; i < size; i++) {
    std::cout << a[i] << std::endl;
  }

  delete [] a;
  delete [] aux;

  return 0;
}

