CC = g++
CFLAGS = -O2 -fopenmp

FF = gfortran
FFLAGS = -O2

all: topk topk_32

topk: topk.cpp
	$(CC) $(CFLAGS) topk.cpp -o topk

topk_32: topk_32.cpp
	$(CC) $(CFLAGS) topk_32.cpp -o topk_32

clean:
	rm -f topk topk_32

# an example of a more complex build line for the Intel icc compiler