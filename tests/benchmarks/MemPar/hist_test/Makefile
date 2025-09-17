CC = g++
CFLAGS = -O2 -fopenmp

FF = gfortran
FFLAGS = -O2

all: hist hist_32

hist: hist.cpp
	$(CC) $(CFLAGS) hist.cpp -o hist

hist_32: hist_32.cpp
	$(CC) $(CFLAGS) hist_32.cpp -o hist_32

clean:
	rm -f hist hist_32

# an example of a more complex build line for the Intel icc compiler