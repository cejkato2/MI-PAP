#code compiler
CC=nvcc
DBG=cuda-gdb

# Add source files here
EXECUTABLE=sort

# Cuda source files (compiled with cudacc)
CUFILES=\
	*.cu

# CUDA dependency files

# C/C++ source files (compiled with gcc / c++)
CCFILES	= \
	

default: build

run: build 
	./$(EXECUTABLE)

dbg: build
	$(DBG) ./$(EXECUTABLE)

build:	$(CCFILES) $(CUFILES)
	$(CC) -g -G -O3 $(CCFILES)  $(CUFILES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
