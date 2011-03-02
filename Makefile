include Makefile.inc

OBJS=obj

all: eomerge sekvencni paralelni

$(OBJS)/merge.o:	merge.cpp
	$(CC) -c merge.cpp -o $(OBJS)/merge.o

$(OBJS)/array.o:	array.cpp
	$(CC) -c array.cpp -o $(OBJS)/array.o

$(OBJS)/2darray.o:	utils/2Darray.cpp
	$(CC) -c utils/2Darray.cpp -o $(OBJS)/2darray.o 

$(OBJS)/loader.o:	utils/loader.cpp
	$(CC) -c utils/loader.cpp -o $(OBJS)/loader.o

eomerge:	$(OBJS)/merge.o $(OBJS)/array.o $(OBJS)/loader.o eomerge.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) eomerge.cpp $(OBJS)/merge.o $(OBJS)/array.o $(OBJS)/loader.o -o eomerge

debug_ms: merge.cpp
	$(CC) $(CFLAGS) -DDEBUG_MERGESORT merge.cpp -o merge

clean: 
	rm -f merge eomerge $(OBJS)/*.o a.out
	cd sekvencni; make clean
	cd paralelni; make clean

sekvencni: force_make 	
	cd sekvencni/; make

paralelni: force_make
	cd paralelni/; make

force_make:
	true
uncrustify:
	uncrustify -c /usr/share/uncrustify/linux-indent.cfg --no-backup --replace *.cpp utils/*.cpp paralelni/*.cpp sekvencni/*.cpp
