CC=g++
CFLAGS=-g -Wall -pedantic
OMPFLAG=-fopenmp

all: eomerge

merge.o:	merge.cpp
	$(CC) -c merge.cpp 

array.o:	array.cpp
	$(CC) -c array.cpp 

loader.o:	utils/loader.cpp
	$(CC) -c utils/loader.cpp -o loader.o

eomerge:	merge.o array.o loader.o eomerge.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) eomerge.cpp merge.o array.o loader.o -o eomerge

debug_ms: merge.cpp
	$(CC) $(CFLAGS) -DDEBUG_MERGESORT merge.cpp -o merge

clean: 
	rm -f merge eomerge *.o

uncrustify:
	uncrustify -c /usr/share/uncrustify/linux-indent.cfg --no-backup --replace *.cpp utils/*.cpp
