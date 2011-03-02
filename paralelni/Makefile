include ../Makefile.inc

OBJS=../obj

shearsort: $(OBJS)/array2D.o $(OBJS)/eot.o shearsort.cpp
	$(CC) $(CFLAGS) shearsort.cpp $(OBJS)/*.o -o shearsort

$(OBJS)/eot.o:	eot.h eot.cpp
	$(CC) -c eot.cpp -o $(OBJS)/eot.o

$(OBJS)/array2D.o: ../utils/2Darray.cpp
	$(CC) -c ../utils/2Darray.cpp -o $(OBJS)/array2D.o

eot_dbg: $(OBJS)/array2D.o eot.cpp
	$(CC) $(CFLAGS) -DDEBUG_EOT eot.cpp $(OBJS)/*.o -o eot

clean:	
	rm -rf eot $(OBJS)/*.o eot shearsort a.out

