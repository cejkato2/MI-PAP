include ../Makefile.inc

OBJS=../obj
BIN=../bin/paralelni

all: $(BIN)/eomergesort $(BIN)/shearsort $(BIN)/bitonicsort

$(BIN)/bitonicsort: bitonicmerge.cpp $(OBJS)/array.o $(OBJS)/2darray.o $(OBJS)/loader.o
	$(CC) $(CFLAGS) $(OMPFLAG) $(OPTFLAG) bitonicmerge.cpp $(OBJS)/array.o $(OBJS)/2darray.o $(OBJS)/loader.o  $(OBJS)/testSort.o -o $(BIN)/bitonicsort


$(BIN)/eomergesort: $(OBJS)/array.o $(OBJS)/loader.o eomergesort.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) eomergesort.cpp $(OBJS)/array.o $(OBJS)/loader.o $(OBJS)/2darray.o $(OBJS)/testSort.o -o $(BIN)/eomergesort

$(BIN)/shearsort: $(OBJS)/2darray.o $(OBJS)/loader.o $(OBJS)/shearmergesort.o shearsort.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) shearsort.cpp $(OBJS)/*.o -o $(BIN)/shearsort

#eot zatim neni potreba
#$(OBJS)/eot.o:	eot.h eot.cpp
#	$(CC) -c eot.cpp -o $(OBJS)/eot.o

$(OBJS)/shearmergesort.o: shearmergesort.h shearmergesort.cpp
	$(CC) -c shearmergesort.cpp -o $(OBJS)/shearmergesort.o

eot_dbg: $(OBJS)/2darray.o eot.cpp
	$(CC) $(CFLAGS) -DDEBUG_EOT eot.cpp $(OBJS)/*.o -o eot

clean:	
	rm -rf eot $(OBJS)/*.o eot shearsort eomergesort a.out bitonicsort

