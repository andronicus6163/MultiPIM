
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
#define USAGE "USAGE: ./dotproduct <num_ele>"



int test_exec(uint64_t num_ele){

  double* ref1 = new double[num_ele];
  double* ref2 = new double[num_ele];

  printf("Address of Input 1: %lx\n", (uint64_t) ref1);
  printf("Address of Input 2: %lx\n", (uint64_t) ref2);
  
  // Initialize the elements 
  double lower_bound = - 1.0;
  double upper_bound = 1.0;
  std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
  std::default_random_engine re;
  for(uint64_t i = 0; i < num_ele; i++){
    auto temp_double = unif(re);
    ref1[i] = temp_double;
    ref2[i] = temp_double;
  }

#ifdef GEM5_MODE
  m5_switch_cpu();
  m5_dump_reset_stats(0,0);
#endif

  double sum = 0.0;
  for (int i = 0; i < num_ele; i++) {
    sum += ref1[i] * ref2[i];
  }

#ifdef GEM5_MODE
  m5_dump_reset_stats(0,0);
#endif

  printf("Dot Product :%f\n", sum);
  //delete[] ref1;
  //delete[] ref2;

  return 0;
}



int main(int argc, char* argv[]) {
  
  uint64_t num_ele = atoll(argv[1]);

  return test_exec(num_ele);
}
