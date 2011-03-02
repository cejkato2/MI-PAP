include ../Makefile.inc

OBJS=../obj

shearsort: $(OBJS)/2darray.o $(OBJS)/eot.o shearsort.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) shearsort.cpp $(OBJS)/*.o -o shearsort

$(OBJS)/eot.o:	eot.h eot.cpp
	$(CC) -c eot.cpp -o $(OBJS)/eot.o

$(OBJS)/2darray.o:	../utils/2Darray.cpp
	$(CC) -c ../utils/2Darray.cpp -o $(OBJS)/2darray.o 

eot_dbg: $(OBJS)/2darray.o eot.cpp
	$(CC) $(CFLAGS) -DDEBUG_EOT eot.cpp $(OBJS)/*.o -o eot

clean:	
	rm -rf eot $(OBJS)/*.o eot shearsort a.out

