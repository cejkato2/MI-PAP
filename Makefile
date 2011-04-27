include Makefile.inc

OBJS=obj

all: sekvencni paralelni

utils: $(OBJS)/merge.o $(OBJS)/array.o $(OBJS)/2darray.o $(OBJS)/loader.o $(OBJS)/testSort.o

$(OBJS)/merge.o:	utils/merge.cpp
	$(CC) -c utils/merge.cpp -o $(OBJS)/merge.o

$(OBJS)/array.o:	utils/array.cpp
	$(CC) -c utils/array.cpp -o $(OBJS)/array.o

$(OBJS)/2darray.o:	utils/2Darray.cpp
	$(CC) -c utils/2Darray.cpp -o $(OBJS)/2darray.o 

$(OBJS)/loader.o:	utils/loader.cpp
	$(CC) -c utils/loader.cpp -o $(OBJS)/loader.o

$(OBJS)/testSort.o:	utils/sortTester.cpp
	$(CC) $(OMPFLAG) -c utils/sortTester.cpp -o $(OBJS)/testSort.o

clean: 
	rm -f testloader $(OBJS)/*.o a.out bin/*/*
	rm -rf tests
	cd sekvencni; make clean
	cd paralelni; make clean

sekvencni: utils force_make 	
	cd sekvencni/ && $(MAKE)

paralelni: utils force_make
	cd paralelni/ && $(MAKE)

force_make:
	true
uncrustify:
	uncrustify -c /usr/share/uncrustify/linux-indent.cfg --no-backup --replace *.cpp utils/*.cpp paralelni/*.cpp sekvencni/*.cpp

to_star:
	utils/transporter.sh

testloader: testloader.cpp
	$(CC) $(CFLAGS) $(OMPFLAG) array.cpp utils/loader.cpp utils/2Darray.cpp testloader.cpp -o testloader

gentests: all
	utils/testmes.sh
reports:
	pdfcslatex -halt-on-error zprava1.tex
	pdfcslatex -halt-on-error zprava2.tex

clean_rep:
	rm -f *.aux *.dvi *.idx *.log *.lot *.out *.toc
