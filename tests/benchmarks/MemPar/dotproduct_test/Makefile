CC = g++
CFLAGS = -O2 -fopenmp

FF = gfortran
FFLAGS = -O2

all: dotproduct dotproduct_32

dotproduct: dotproduct.cpp
	$(CC) $(CFLAGS) dotproduct.cpp -o dotproduct

dotproduct_32: dotproduct_32.cpp
	$(CC) $(CFLAGS) dotproduct_32.cpp -o dotproduct_32

clean:
	rm -f dotproduct dotproduct_32

# an example of a more complex build line for the Intel icc compiler