
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#ifdef GEM5_MODE
# include <gem5/m5ops.h>
#endif

#define USAGE "USAGE: ./fft <num_ele>"

#define PI 3.14159265358979323846

typedef struct {
    double real;
    double imag;
} Complex;

// Function to perform the FFT
void fft(Complex* X, int N) {
    // Base case
    if (N <= 1) return;

    // Divide
    Complex* X_even = (Complex*) malloc(N/2 * sizeof(Complex));
    Complex* X_odd = (Complex*) malloc(N/2 * sizeof(Complex));
    for (int i = 0; i < N/2; i++) {
        X_even[i] = X[i*2];
        X_odd[i] = X[i*2 + 1];
    }

    // Conquer
    fft(X_even, N/2);
    fft(X_odd, N/2);

    // Combine
    for (int k = 0; k < N/2; k++) {
        double x = -2.0 * PI * k / N;
        double c = 1 - x * x / 2 + x * x * x * x / 24;
        double s = x - x * x * x / 6 + x * x * x * x * x / 120;

        double t_real = c * X_odd[k].real - s * X_odd[k].imag;
        double t_imag = s * X_odd[k].real + c * X_odd[k].imag;
        
        X[k].real = X_even[k].real + t_real;
        X[k].imag = X_even[k].imag + t_imag;
        X[k + N/2].real = X_even[k].real - t_real;
        X[k + N/2].imag = X_even[k].imag - t_imag;
    }

    // Clean up
    free(X_even);
    free(X_odd);
}

int test_exec(uint64_t num_ele){

  Complex* ref1 = new Complex[2 * num_ele];

  printf("Address of Input 1: %lx\n", (uint64_t) ref1);

    // Initialize the elements 
  double lower_bound = - 1.0;
  double upper_bound = 1.0;
  std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
  std::default_random_engine re;
  for(uint64_t i = 0; i < 2 * num_ele; i++){
    auto temp_double = unif(re);
    ref1[i].real = temp_double;
    ref1[i].imag = temp_double;
  }

#ifdef GEM5_MODE
  m5_switch_cpu();
  m5_dump_reset_stats(0,0);
#endif

  fft(ref1, num_ele);

#ifdef GEM5_MODE
  m5_dump_reset_stats(0,0);
#endif
  
  delete[] ref1;
  
  return 0;
}



int main(int argc, char* argv[]) {
  
  uint64_t num_ele = atoll(argv[1]);

  return test_exec(num_ele);
}
