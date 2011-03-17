include ../Makefile.inc

OBJS=../obj

default: bitonic_ms eomerge

#kompilovani debug bitonic sortu
bitonic_ms: bitonicmerge.cpp $(OBJS)/array.o $(OBJS)/2darray.o $(OBJS)/loader.o
	$(CC) $(CFLAGS) $(OMPFLAG) bitonicmerge.cpp $(OBJS)/array.o $(OBJS)/2darray.o $(OBJS)/loader.o -o bmerge

eomerge: eodmerge.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) eodmerge.cpp -o eodmerge

clean: 
	rm -f bmerge eodmerge *.o a.out

