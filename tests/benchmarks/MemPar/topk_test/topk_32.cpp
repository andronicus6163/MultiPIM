
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <random>

#define USAGE "USAGE: ./topk <num_ele>"

#define TOPK 8

#define RSEED 1
typedef struct {
    double value;
    uint64_t index;
} ValueIndexPair;

void parallel_part(ValueIndexPair* ref1, ValueIndexPair* ref2, uint64_t num_ele, int cpu_id) {

  uint64_t min_index = 0;
  double min_value = 0.0;

  for (int i = 0; i < num_ele / 32; i++) {
    if (i < TOPK) {
      ref2[cpu_id * TOPK + i].value = ref1[i].value;
      ref2[cpu_id * TOPK + i].index = ref1[i].index;
      if (i == 0) {
        min_index = 0;
        min_value = ref1[0].value;
      } else {
        if (ref1[i].value < min_value) {
          min_index = i;
          min_value = ref1[i].value;
        }
      }
    } else {
      if (ref1[i].value > min_value) {
        ref2[cpu_id * TOPK + min_index].value = ref1[i].value;
        ref2[cpu_id * TOPK + min_index].index = ref1[i].index;
        min_index = 0;
        min_value = ref2[cpu_id * TOPK + 0].value;
        for (int j = 1; j < TOPK; j++) {
          if (ref2[cpu_id * TOPK + j].value < min_value) {
            min_index = j;
            min_value = ref2[cpu_id * TOPK + j].value;
          }
        }
      }
    }
  }
}


int test_exec(uint64_t num_ele, int cpu_id){

  ValueIndexPair* ref1 = new ValueIndexPair[num_ele / 32];
  ValueIndexPair* ref2 = new ValueIndexPair[32 * TOPK];
  ValueIndexPair* ref3 = new ValueIndexPair[TOPK];

  printf("Address of Input 1: %lx\n", (uint64_t) ref1);
  printf("Address of Input 2: %lx\n", (uint64_t) ref2);
  
  // Initialize the elements 
  double lower_bound = - 1.0;
  double upper_bound = 1.0;
  std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
  std::default_random_engine re;
  for(uint64_t i = 0; i < num_ele / 32; i++){
    auto temp_double = unif(re);
    ref1[i].value = temp_double;
    ref1[i].index = i;
  }

  for(uint64_t i = 0; i < 32 * TOPK; i++){
    auto temp_double = unif(re);
    ref2[i].value = temp_double;
    ref2[i].index = i;
  }

  //srand(RSEED);
  //for(int i = 0; i < num_ele; i++){
  //  ref1[i].index = i;
  //  ref1[i].value = rand() % 256;
  //}

#ifdef GEM5_MODE
  m5_switch_cpu();
  m5_dump_reset_stats(0,0);
#endif

  parallel_part(ref1, ref2, num_ele, cpu_id);

  if (cpu_id == 0) {
    uint64_t min_index = 0;
    double min_value = 0.0;

    for (int i = 0; i < num_ele / 32; i++) {
      if (i < TOPK) {
        ref3[i].value = ref2[i].value;
        ref3[i].index = ref2[i].index;
        if (i == 0) {
          min_index = 0;
          min_value = ref3[0].value;
        } else {
          if (ref3[i].value < min_value) {
            min_index = i;
            min_value = ref3[i].value;
          }
        }
      } else {
        if (ref2[i].value > min_value) {
          ref3[min_index].value = ref2[i].value;
          ref3[min_index].index = ref2[i].index;
          min_index = 0;
          min_value = ref3[0].value;
          for (int j = 1; j < TOPK; j++) {
            if (ref3[j].value < min_value) {
              min_index = j;
              min_value = ref3[j].value;
            }
          }
        }
      }
    }
  }


#ifdef GEM5_MODE
  m5_dump_reset_stats(0,0);
#endif
  
  delete[] ref1;
  delete[] ref2;

  return 0;
}



int main(int argc, char* argv[]) {
  
  uint64_t num_ele = atoll(argv[1]);
  int cpu_id = atoi(argv[2]);

  return test_exec(num_ele, cpu_id);
}
