
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
#define USAGE "USAGE: ./spmv <file_path> <num_workers>"



int test_exec(std::string filename, uint64_t num_workers) {

  std::ifstream mat_file(filename);
  if(!mat_file.is_open()){
    printf("Failed to open matrix file!\n");
    exit(1);
  }
  
  std::string line;
  std::getline(mat_file, line);
  std::istringstream iss(line);
  std::string result;
  std::string token;
  std::getline( iss, token, ' ' );
  int h = std::stoi(token);
  std::getline( iss, token, ' ' );
  int w = std::stoi(token);
  std::getline( iss, token, ' ' );
  int nnz = std::stoi(token);

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

  uint64_t prev_idx = 0;
  for(int i = 0; i < h; i ++){
    std::getline(mat_file, line);
    mat1_index[i * 2] = prev_idx;
    mat1_index[i * 2 + 1] = std::stoi(line);
    prev_idx = mat1_index[i * 2 + 1];
  }

  for(int i = 0; i < nnz; i ++){
    std::getline(mat_file, line);
    mat1_cols[i] = std::stoi(line);
  }

  for(int i = 0; i < nnz; i ++){
    std::getline(mat_file, line);
    mat1_vals[i] = std::stod(line);
  }

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

  for (int i = 0; i < h; i++) {
    double sum = 0.0;
    for (int j = mat1_index[i * 2] / 8; j < mat1_index[i * 2 + 1] / 8; j++) {
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
  uint64_t num_workers = std::stoi(argv[2]);

  return test_exec(path, num_workers);
}
