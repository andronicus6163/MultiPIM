CC = g++
CFLAGS = -O2 -fopenmp

FF = gfortran
FFLAGS = -O2

all: cnn cnn_32

cnn: cnn.cpp
	$(CC) $(CFLAGS) cnn.cpp -o cnn

cnn_32: cnn_32.cpp
	$(CC) $(CFLAGS) cnn_32.cpp -o cnn_32

clean:
	rm -f cnn cnn_32

# an example of a more complex build line for the Intel icc compiler