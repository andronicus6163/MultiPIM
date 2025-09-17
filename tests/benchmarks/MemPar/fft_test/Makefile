CC = g++
CFLAGS = -O2 -fopenmp

FF = gfortran
FFLAGS = -O2

all: fft

fft: fft.cpp
	$(CC) $(CFLAGS) fft.cpp -o fft

clean:
	rm -f fft

# an example of a more complex build line for the Intel icc compiler