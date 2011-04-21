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
	

default: build_dbg

run: build 
	./$(EXECUTABLE)

dbg: build_dbg
	$(DBG) ./$(EXECUTABLE)

dbg_run: build_dbg
	./$(EXECUTABLE)

#compile optimized version without debug
build:	$(CCFILES) $(CUFILES)
	$(CC) -O3 $(CCFILES)  $(CUFILES) -o $(EXECUTABLE)


build_dbg: $(CCFILES) $(CUFILES)
	$(CC) -g -G -DDEBUG_GLOBAL $(CCFILES)  $(CUFILES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) .*.swp
