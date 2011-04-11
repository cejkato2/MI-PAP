#code compiler
CC=nvcc

# Add source files here
EXECUTABLE=sort

# Cuda source files (compiled with cudacc)
CUFILES= \
	main.cu \

# CUDA dependency files

# C/C++ source files (compiled with gcc / c++)
CCFILES	= \
	

default: build


build:	$(CCFILES) $(CUFILES)
	$(CC) -g -G $(CCFILES)  $(CUFILES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
