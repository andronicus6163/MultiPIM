
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

void parallel_part(uint64_t* ref, uint64_t* ref_output, uint64_t num_ele, int cpu_id) {
  for (int i = 0; i < HIST_BIN; i++) {
    ref_output[cpu_id] = 0;
  }

  for (int i = 0; i < num_ele / 32; i++) {
    ref_output[cpu_id * HIST_BIN + ref[i] >> (64 - HIST_BIT)] = ref_output[cpu_id * 32 + ref[i] >> (64 - HIST_BIT)] + 1;
  }
}


int test_exec(uint64_t num_ele, int cpu_id){

  uint64_t* ref = new uint64_t[num_ele / 32];
  uint64_t* ref_output = new uint64_t[32 * HIST_BIN];

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
  for (uint64_t i = 0; i < num_ele / 32; i++) {
    auto temp_long = unif(re);
    ref[i] = temp_long;
  }

#ifdef GEM5_MODE
  m5_switch_cpu();
  m5_dump_reset_stats(0,0);
#endif

  parallel_part(ref, ref_output, num_ele, cpu_id);

  if (cpu_id == 0) {
    for (int i = 0; i < 32; i++) {
      for (int j = 0; j < HIST_BIN; j++) {
        ref_output[j] = ref_output[j] + ref_output[i * HIST_BIN + j];
      }
    }
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
  int cpu_id = atoi(argv[2]);

  return test_exec(num_ele, cpu_id);
}
