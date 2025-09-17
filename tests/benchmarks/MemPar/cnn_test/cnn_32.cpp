
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <random>

#define USAGE "USAGE: ./cnn <num_edge_ele>"

#define KERNEL_SIZE 3

#define INPUT_TILE_SIZE 10
#define OUTPUT_TILE_SIZE 8


void parallel_part(double* ref1, double* ref2, double* ref3, int num_ele) {
  double tile1[INPUT_TILE_SIZE * INPUT_TILE_SIZE];
  double tile2[OUTPUT_TILE_SIZE * OUTPUT_TILE_SIZE];
  for (int i = 0; i < (num_ele - KERNEL_SIZE + 1) / OUTPUT_TILE_SIZE / 32; i++) {
    for (int j = 0; j < (num_ele - KERNEL_SIZE + 1) / OUTPUT_TILE_SIZE; j++) {
      for (int k = 0; k < INPUT_TILE_SIZE; k++) {
        for (int l = 0; l < INPUT_TILE_SIZE; l++) {
          tile1[k * INPUT_TILE_SIZE + l] = ref1[(i * OUTPUT_TILE_SIZE + k) * num_ele + (j * OUTPUT_TILE_SIZE + l)];
        }
      }
      for (int k = 0; k < OUTPUT_TILE_SIZE; k++) {
        for (int l = 0; l < OUTPUT_TILE_SIZE; l++) {
          double sum = 0.0;
          for (int m = 0; m < KERNEL_SIZE; m++) {
            for (int n = 0; n < KERNEL_SIZE; n++) {
              sum += ref2[m * KERNEL_SIZE + n] * tile1[(k + m) * INPUT_TILE_SIZE + (l + n)];
            }
          }
          tile2[k * OUTPUT_TILE_SIZE + l] = sum;
        }
      }
      for (int k = 0; k < OUTPUT_TILE_SIZE; k++) {
        for (int l = 0; l < OUTPUT_TILE_SIZE; l++) {
          ref3[(i * OUTPUT_TILE_SIZE + k) * (num_ele - KERNEL_SIZE + 1) + (j * OUTPUT_TILE_SIZE + l)] = tile2[k * OUTPUT_TILE_SIZE + l];
        }
      }
    }
  }
}


int test_exec(uint64_t num_ele){

  double* ref1 = new double[num_ele * ((num_ele - KERNEL_SIZE + 1) / 32 + KERNEL_SIZE - 1)];
  double* ref2 = new double[KERNEL_SIZE * KERNEL_SIZE];
  double* ref3 = new double[(num_ele - KERNEL_SIZE + 1) * (num_ele - KERNEL_SIZE + 1) / 32];
  // Initialize the elements 
  double lower_bound = - 1.0;
  double upper_bound = 1.0;
  std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
  std::default_random_engine re;
  for(uint64_t i = 0; i < num_ele * ((num_ele - KERNEL_SIZE + 1) / 32 + KERNEL_SIZE - 1); i++){
    auto temp_double = unif(re);
    ref1[i] = temp_double;
  }
  for(uint64_t i = 0; i < KERNEL_SIZE * KERNEL_SIZE; i++){
    auto temp_double = unif(re);
    ref2[i] = temp_double;
  }
  
  for(uint64_t i = 0; i < (num_ele - KERNEL_SIZE + 1) * (num_ele - KERNEL_SIZE + 1) / 32; i++){
    ref3[i] = 0;
  }


  printf("Address of Input 1: %lx\n", (uint64_t) ref1);
  printf("Address of Input 2: %lx\n", (uint64_t) ref2);
  printf("Address of Output: %lx\n", (uint64_t) ref3);

#ifdef GEM5_MODE
  m5_switch_cpu();
  m5_dump_reset_stats(0,0);
#endif
// for (int i = 0; i < 32; i++) {
//   parallel_part(ref1, ref2, ref3, num_ele, i);
// }
  parallel_part(ref1, ref2, ref3, num_ele);

#ifdef GEM5_MODE
  m5_dump_reset_stats(0,0);
#endif
  
  delete[] ref1;
  delete[] ref2;
  delete[] ref3;
  
  return 0;
}



int main(int argc, char* argv[]) {
  
  uint64_t num_edge_ele = atoll(argv[1]);

  return test_exec(num_edge_ele);
}
