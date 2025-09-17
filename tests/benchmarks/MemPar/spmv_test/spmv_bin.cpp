
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
#define USAGE "USAGE: ./spmv_bin <file_path> <num_workers>"



int test_exec(std::string filename) {

  std::ifstream mat_file(filename, std::ios::in | std::ios::binary);
  if(!mat_file.is_open()){
    printf("Failed to open matrix file!\n");
    exit(1);
  }
  
  uint64_t h, w, nnz, pad_len;
  mat_file.read(reinterpret_cast<char*>(&h), sizeof(uint64_t));
  mat_file.read(reinterpret_cast<char*>(&w), sizeof(uint64_t));
  mat_file.read(reinterpret_cast<char*>(&nnz), sizeof(uint64_t));
  mat_file.read(reinterpret_cast<char*>(&pad_len), sizeof(uint64_t));

  printf("Received Matrix (Height: %ld  Width: %ld  Nonzeros: %ld Pad Len: %ld)\n", h, w, nnz, pad_len);

  if(nnz != pad_len){
    printf("ERROR: Not a baseline dump\n");
    exit(1);
  }

  uint64_t* mat1_cols = new uint64_t[nnz];
  double*   mat1_vals = new double[nnz];
  uint64_t* mat1_index = new uint64_t[2 * h];

  double*   mat2 = new double[w];

  double*   result_vector = new double[h];

  printf("Address of mat1_cols: %lx\n", (uint64_t) mat1_cols);
  printf("Address of mat1_vals: %lx\n", (uint64_t) mat1_vals);
  printf("Address of mat1_index: %lx\n", (uint64_t) mat1_index);
  printf("Address of mat2: %lx\n", (uint64_t) mat2);
  printf("Address of result_vector: %lx\n", (uint64_t) result_vector);

  mat_file.read(reinterpret_cast<char*>(mat1_index), 2 * h * sizeof(uint64_t));
  printf("mat1_index[0] = %ld\n", mat1_index[0]);

  mat_file.read(reinterpret_cast<char*>(mat1_cols), pad_len * sizeof(uint64_t));
  printf("mat1_cols[0] = %ld\n", mat1_cols[0]);

  mat_file.read(reinterpret_cast<char*>(mat1_vals), pad_len * sizeof(double));
  printf("mat1_vals[0] = %lf\n", mat1_vals[0]);

  printf("Address of Matrix 1: %lx\n", (uint64_t) mat1_vals);

  printf("Building Matrix 2\n");

  for(int i = 0; i < w; i ++){
    mat2[i] = 1.0;
  }

  // To make sure result_vector is also in cache for the non cache experiments
  for(int i = 0; i < h; i++){
    result_vector[i] = 0; 
  }

#ifdef GEM5_MODE
  m5_switch_cpu();
  m5_dump_reset_stats(0,0);
#endif

  for (uint64_t i = 0; i < h; i++) {
    double sum = 0.0;
    for (uint64_t j = mat1_index[i * 2]; j < mat1_index[i * 2 + 1]; j++) {
      sum += mat1_vals[j] * mat2[mat1_cols[j]];
    }
    result_vector[i] = sum;
  }

#ifdef GEM5_MODE
  m5_dump_reset_stats(0,0);
#endif

  double sum = 0.0;
  for (int i = 0; i < h; i++) {
    sum += result_vector[i];
  }
  printf("spmv finish with sum :%f\n", sum);
  // delete[] mat1_cols;
  // delete[] mat1_vals;
  // delete[] mat1_index;
  // delete[] result_vector;

  return 0;
}



int main(int argc, char* argv[]) {
  
  std::string path = argv[1];

  return test_exec(path);
}
