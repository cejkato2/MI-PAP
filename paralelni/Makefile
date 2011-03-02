CC=g++
CFLAGS=-g -fopenmp -Wall -pedantic

shearsort: array2D.o eot.o shearsort.cpp
	$(CC) $(CFLAGS) shearsort.cpp *.o -o shearsort

eot.o:	eot.h eot.cpp
	$(CC) -c eot.cpp

array2D.o: ../utils/2Darray.cpp
	$(CC) -c ../utils/2Darray.cpp 

eot_dbg: array2D.o eot.cpp
	$(CC) $(CFLAGS) -DDEBUG_EOT eot.cpp *.o -o eot

clean:	
	rm -rf eot *.o eot shearsort

uncrustify:
	uncrustify -c /usr/share/uncrustify/linux-indent.cfg --no-backup --replace *.cpp

