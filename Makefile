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

eomerge:	$(OBJS)/merge.o $(OBJS)/array.o $(OBJS)/loader.o $(OBJS)/2darray.o eomerge.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) eomerge.cpp $(OBJS)/merge.o $(OBJS)/array.o $(OBJS)/loader.o $(OBJS)/2darray.o -o eomerge

debug_ms: merge.cpp
	$(CC) $(CFLAGS) -DDEBUG_MERGESORT merge.cpp -o merge

clean: 
	rm -f merge eomerge testloader $(OBJS)/*.o a.out
	cd sekvencni; make clean
	cd paralelni; make clean

sekvencni: force_make 	
	cd sekvencni/ && $(MAKE)

paralelni: force_make
	cd paralelni/ && $(MAKE)

force_make:
	true
uncrustify:
	uncrustify -c /usr/share/uncrustify/linux-indent.cfg --no-backup --replace *.cpp utils/*.cpp paralelni/*.cpp sekvencni/*.cpp

to_star:
	utils/transporter.sh

testloader: testloader.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) array.cpp utils/loader.cpp utils/2Darray.cpp testloader.cpp -o testloader
