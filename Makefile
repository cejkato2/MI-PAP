CC=g++
CFLAGS=-fopenmp -g -Wall -pedantic

all: merge.o eomerge

merge.o:	merge.cpp
	$(CC) -c merge.cpp 

eomerge:	merge.o eomerge.cpp
	$(CC) $(CFLAGS) eomerge.cpp merge.o -o eomerge

debug_ms: merge.cpp
	$(CC) $(CFLAGS) -DDEBUG_MERGESORT merge.cpp -o merge

clean: 
	rm -f merge eomerge *.o

uncrustify:
	uncrustify -c /usr/share/uncrustify/linux-indent.cfg --no-backup --replace *.cpp
