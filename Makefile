	CC=g++
CFLAGS=-Wall -pedantic

merge:	merge.cpp
	$(CC) $(CFLAGS) merge.cpp -o merge

clean: 
	rm -f merge *.o

uncrustify:
	uncrustify -c /usr/share/uncrustify/linux-indent.cfg --no-backup --replace *.cpp
