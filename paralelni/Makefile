include ../Makefile.inc

OBJS=../obj
BIN=../bin/paralelni

all: $(BIN)/eomergesort $(BIN)/shearsort $(BIN)/bitonicsort

$(BIN)/bitonicsort: bitonicmerge.cpp $(OBJS)/array.o $(OBJS)/2darray.o $(OBJS)/loader.o $(OBJS)/eot.o
	$(CC) $(CFLAGS) $(OMPFLAG) $(OPTFLAG) bitonicmerge.cpp $(OBJS)/array.o $(OBJS)/2darray.o $(OBJS)/loader.o $(OBJS)/eot.o $(OBJS)/testSort.o -o $(BIN)/bitonicsort


$(BIN)/eomergesort: $(OBJS)/array.o $(OBJS)/loader.o eomergesort.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) eomergesort.cpp $(OBJS)/array.o $(OBJS)/loader.o $(OBJS)/2darray.o $(OBJS)/testSort.o -o $(BIN)/eomergesort

$(BIN)/shearsort: $(OBJS)/2darray.o $(OBJS)/loader.o $(OBJS)/eot.o shearsort.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) shearsort.cpp $(OBJS)/*.o -o $(BIN)/shearsort

$(OBJS)/eot.o:	eot.h eot.cpp
	$(CC) -c eot.cpp -o $(OBJS)/eot.o

eot_dbg: $(OBJS)/2darray.o eot.cpp
	$(CC) $(CFLAGS) -DDEBUG_EOT eot.cpp $(OBJS)/*.o -o eot

clean:	
	rm -rf eot $(OBJS)/*.o eot shearsort eomergesort a.out bitonicsort

