CC=g++
CFLAGS=-g -fopenmp -Wall -pedantic

default: build

build:	eot.cpp
	$(CC) $(CFLAGS) eot.cpp -o eot

rbuild: clean build
	./eot

clean:	
	rm -rf eot *.o

uncrustify:
	uncrustify -c /usr/share/uncrustify/linux-indent.cfg --no-backup --replace *.cpp

