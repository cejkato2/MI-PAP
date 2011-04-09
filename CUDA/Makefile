#code compiler
CC=nvcc

# Add source files here
EXECUTABLE=sort

# Cuda source files (compiled with cudacc)
CUFILES=\
        main.cu \

# CUDA dependency files

# C/C++ source files (compiled with gcc / c++)
CCFILES	=\

default: build

run: clean build
	echo "Starting up the program."
	./$(EXECUTABLE)

build:	$(CCFILES) $(CUFILES)
	echo "Building CUDA"
	$(CC) -g -G $(CCFILES)  $(CUFILES) -o $(EXECUTABLE)

clean:
	echo "Cleaning directories."
	rm -f $(EXECUTABLE) *.out
