include ../Makefile.inc

OBJS=../obj
BIN=../bin/sekvencni

default: $(BIN)/bitonic_ms $(BIN)/mergesort

#kompilovani debug bitonic sortu
$(BIN)/bitonic_ms: bitonicmerge.cpp $(OBJS)/array.o $(OBJS)/2darray.o $(OBJS)/loader.o
	$(CC) $(CFLAGS) $(OMPFLAG) bitonicmerge.cpp $(OBJS)/array.o $(OBJS)/2darray.o $(OBJS)/loader.o -o $(BIN)/bitonic_ms

eomerge: eodmerge.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) eodmerge.cpp -o eodmerge




$(BIN)/mergesort: mergesort.cpp $(OBJS)/testSort.o $(OBJS)/merge.o $(OBJS)/array.o $(OBJS)/loader.o $(OBJS)/2darray.o
	$(CC) $(CFLAGS) $(OMPFLAG) $(OBJS)/testSort.o $(OBJS)/merge.o $(OBJS)/array.o $(OBJS)/loader.o $(OBJS)/2darray.o mergesort.cpp -o $(BIN)/mergesort

debug_ms: merge.cpp
	$(CC) $(CFLAGS) -DDEBUG_MERGESORT merge.cpp -o merge

clean: 
	rm -f bitonic_ms eodmerge mergesort *.o a.out

