import os


num_threads = "20"
MAT1_DIM = 256
# Ensure we use 4 CPU cores (works for OpenBLAS and Intel MKL)
os.environ["OMP_NUM_THREADS"] = num_threads
os.environ["OPENBLAS_NUM_THREADS"] = num_threads
os.environ["MKL_NUM_THREADS"] = num_threads
os.environ["VECLIB_MAXIMUM_THREADS"] = num_threads
os.environ["NUMEXPR_NUM_THREADS"] = num_threads

# os.environ["GOMP_CPU_AFFINITY"] = "0-20"
os.environ["KMP_AFFINITY"] = "granularity=fine,compact,1,0"

from mkl_fft import fft2, ifft2
import numpy as np
import sys

import time


def main(mat_size_x, mat_size_y):
    print(f"Generating matrix of size {mat_size_x}x{mat_size_y}...")
    data = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    data0 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    data1 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    data2 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    data3 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    data4 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    data5 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    data6 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    data7 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    data8 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    data9 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    # data10 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    # data11 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    # data12 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    # data13 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    # data14 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    # data15 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    # data16 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    # data17 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    # data18 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)
    # data19 = np.random.rand(mat_size_x, mat_size_y) + 1j*np.random.rand(mat_size_x, mat_size_y)

    result = fft2(data)
    print("Performing 2D FFT...")
    # Measure execution time
    num_trials=10

    start_time = time.process_time()
    # for _ in range(num_trials):
    #     result = fft2(data)
    result0 = fft2(data0)
    result1 = fft2(data1)
    result2 = fft2(data2)
    result3 = fft2(data3)
    result4 = fft2(data4)
    result5 = fft2(data5)
    result6 = fft2(data6)
    result7 = fft2(data7)
    result8 = fft2(data8)
    result9 = fft2(data9)
    # result10 = fft2(data10)
    # result11 = fft2(data11)
    # result12 = fft2(data12)
    # result13 = fft2(data13)
    # result14 = fft2(data14)
    # result15 = fft2(data15)
    # result16 = fft2(data16)
    # result17 = fft2(data17)
    # result18 = fft2(data18)
    # result19 = fft2(data19)

    end_time = time.process_time()

    # Compute average execution time
    avg_time = (end_time - start_time) / num_trials
    
    print(f"2D FFT Matrix Size: {mat_size_x}x{mat_size_y}, Avg Time: {avg_time:.6f} sec")
    return avg_time
    



if __name__ == "__main__":
    mat_size_x = int(sys.argv[1])
    mat_size_y = int(sys.argv[2])

    main(mat_size_x, mat_size_y)
    
    
