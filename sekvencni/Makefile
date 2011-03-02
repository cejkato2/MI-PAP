include ../Makefile.inc

OBJS=../obj

default: bitonic_ms

#kompilovani debug bitonic sortu
bitonic_ms: bitonicmerge.cpp
	$(CC) $(CFLAGS)  bitonicmerge.cpp -o bmerge

eomerge: eodmerge.cpp
	$(CC) $(CFLAGS) eodmerge.cpp -o eodmerge

clean: 
	rm -f bmerge eodmerge *.o a.out

