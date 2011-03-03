include ../Makefile.inc

OBJS=../obj

default: bitonic_ms eomerge

#kompilovani debug bitonic sortu
bitonic_ms: bitonicmerge.cpp $(OBJS)/array.o $(OBJS)/2darray.o $(OBJS)/loader.o
	$(CC) $(CFLAGS)  bitonicmerge.cpp $(OBJS)/array.o $(OBJS)/2darray.o $(OBJS)/loader.o -o bmerge

eomerge: eodmerge.cpp
	$(CC) $(CFLAGS) eodmerge.cpp -o eodmerge

clean: 
	rm -f bmerge eodmerge *.o a.out

