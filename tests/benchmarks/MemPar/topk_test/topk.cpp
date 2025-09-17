
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

int test_exec(uint64_t num_ele){

  ValueIndexPair* ref1 = new ValueIndexPair[num_ele];
  ValueIndexPair* ref2 = new ValueIndexPair[TOPK];

  printf("Address of Input 1: %lx\n", (uint64_t) ref1);
  printf("Address of Input 2: %lx\n", (uint64_t) ref2);
  
  // Initialize the elements 
  double lower_bound = - 1.0;
  double upper_bound = 1.0;
  std::uniform_real_distribution<double> unif(lower_bound,upper_bound);
  std::default_random_engine re;
  for(uint64_t i = 0; i < num_ele; i++){
    auto temp_double = unif(re);
    ref1[i].value = temp_double;
    ref1[i].index = i;
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

  uint64_t min_index = 0;
  double min_value = 0.0;

  for (int i = 0; i < num_ele; i++) {
    if (i < TOPK) {
      ref2[i].value = ref1[i].value;
      ref2[i].index = ref1[i].index;
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
        ref2[min_index].value = ref1[i].value;
        ref2[min_index].index = ref1[i].index;
        min_index = 0;
        min_value = ref2[0].value;
        for (int j = 1; j < TOPK; j++) {
          if (ref2[j].value < min_value) {
            min_index = j;
            min_value = ref2[j].value;
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

  return test_exec(num_ele);
}
