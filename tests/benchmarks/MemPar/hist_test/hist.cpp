
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
#define RSEED 1
#define USAGE "USAGE: ./hist <num_ele>"

#define HIST_BIN 256
#define HIST_BIT 8

int test_exec(uint64_t num_ele){

  uint64_t* ref = new uint64_t[num_ele];
  uint64_t* ref_output = new uint64_t[HIST_BIN];

  printf("Address of Input 1: %lx\n", (uint64_t) ref);
  printf("Address of Input 2: %lx\n", (uint64_t) ref_output);
  uint64_t lower_bound = 0;
  uint64_t upper_bound = 0xFFFF'FFFF'FFFF'FFFF;
  std::uniform_int_distribution<uint64_t> unif(lower_bound, upper_bound);
  std::default_random_engine re;
  // Initialize the elements 
  //srand(RSEED);
  //for(int i = 0; i < num_ele; i++){
  //  ref[i] = rand() % 256;
  //}
  for (uint64_t i = 0; i < num_ele; i++) {
    auto temp_long = unif(re);
    ref[i] = temp_long;
  }

#ifdef GEM5_MODE
  m5_switch_cpu();
  m5_dump_reset_stats(0,0);
#endif

  for (int i = 0; i < HIST_BIN; i++) {
      ref_output[i] = 0;
  }

  for (int i = 0; i < num_ele; i++) {
    ref_output[ref[i] >> (64 - HIST_BIT)]++;
  }

#ifdef GEM5_MODE
  m5_dump_reset_stats(0,0);
#endif
  
  delete[] ref;
  delete[] ref_output;

  return 0;
}



int main(int argc, char* argv[]) {
  
  uint64_t num_ele = atoll(argv[1]);

  return test_exec(num_ele);
}
