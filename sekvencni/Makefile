CC=g++
CFLAGS=-g -Wall -pedantic
OMPFLAG=-fopenmp

default: bitonic_ms

#kompilovani debug bitonic sortu
bitonic_ms: bitonicmerge.cpp
	$(CC) $(CFLAGS)  bitonicmerge.cpp -o bmerge

eomerge: eodmerge.cpp
	$(CC) $(CFLAGS) eodmerge.cpp -o eodmerge

clean: 
	rm -f bmerge eodmerge *.o

uncrustify:
	uncrustify -c /usr/share/uncrustify/linux-indent.cfg --no-backup --replace *.cpp
