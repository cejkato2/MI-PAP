include ../Makefile.inc

OBJS=../obj

all: eomergesort shearsort

eomergesort: $(OBJS)/array.o $(OBJS)/loader.o eomergesort.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) eomergesort.cpp $(OBJS)/*.o -o eomergesort

shearsort: $(OBJS)/2darray.o $(OBJS)/loader.o $(OBJS)/eot.o shearsort.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) shearsort.cpp $(OBJS)/*.o -o shearsort

$(OBJS)/eot.o:	eot.h eot.cpp
	$(CC) -c eot.cpp -o $(OBJS)/eot.o

$(OBJS)/2darray.o:	../utils/2Darray.cpp
	$(CC) -c ../utils/2Darray.cpp -o $(OBJS)/2darray.o 

eot_dbg: $(OBJS)/2darray.o eot.cpp
	$(CC) $(CFLAGS) -DDEBUG_EOT eot.cpp $(OBJS)/*.o -o eot

clean:	
	rm -rf eot $(OBJS)/*.o eot shearsort eomergesort a.out

