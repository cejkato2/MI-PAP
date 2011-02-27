CC=g++
CFLAGS=-g -Wall -pedantic
OMPFLAG=-fopenmp

default: bitonic_ms

#kompilovani debug bitonic sortu
bitonic_ms: bitonicmerge.cpp
	$(CC) $(CFLAGS)  bitonicmerge.cpp -o bmerge

clean: 
	rm -f bmerge *.o

uncrustify:
	uncrustify -c /usr/share/uncrustify/linux-indent.cfg --no-backup --replace *.cpp
