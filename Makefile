CC=g++
CFLAGS=-Wall -pedantic

merge:	merge.cpp
	$(CC) $(CFLAGS) merge.cpp -o merge

clean: 
	rm -f merge *.o
