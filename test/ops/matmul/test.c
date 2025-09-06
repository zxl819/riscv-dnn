#include <stdio.h>
#include <stdlib.h>
#include "../../../src/matmul_rvm_int8.h"
#include "../include/matrix/matrix_intrinsic.h"
#include "tensor.h"
#include <riscv_matrix.h>
#include <stddef.h>
#include <assert.h>

#define M 64
#define K 64
#define N 64
// #define A 0
// #define B 4096
// #define C 8192

/* 其中.matA,B,C由link.ld中定义
  . = 0x80040000; _matA_start = .; .matA : { *(.matA) } _matA_end = .;
  . = 0x80041000; _matB_start = .; .matB : { *(.matB) } _matB_end = .;
  . = 0x80042000; _matC_start = .; .matC : { *(.matC) } _matC_end = .;*/

__attribute__((section(".matA"), aligned(64))) int8_t  A[M*K];
__attribute__((section(".matB"), aligned(64))) int8_t  B[K*N];
__attribute__((section(".matC"), aligned(64))) int32_t C[M*N];



int main()
{

// int8_t A_DATA;
// int8_t B_DATA;
// int8_t C_DATA;
// &A=A;
// &B=B;
// &C=C;
static inline int matmul_batch1( A, B,C,
                        int M, int N, int K){
  int m = M, n = N, k = K;
  // assert(src1->shape[0] == m && src1->shape[1] == k);
  // assert(src2->shape[0] == k && src2->shape[1] == n);
  // assert(dst->shape[0]  == m && dst->shape[1]  == n);

  // int8_t  *A ;
  // int8_t  *B ;
  // int32_t *C ;

  const int lda_bytes = k * (int)sizeof(int8_t);
  const int ldb_bytes = n * (int)sizeof(int8_t);
  const int ldc_bytes = n * (int)sizeof(int32_t);

  const int dataSize = sizeof(int8_t);
  int tile_m = 0, tile_n = 0, tile_k = 0;

  for (int i = 0; i < m; i += tile_m) {
    tile_m = msettilem(m - i);
    for (int j = 0; j < n; j += tile_n) {
      tile_n = msettilen(n - j);
      mint32_t acc;
      for (int kk = 0; kk < k; kk += tile_k) {
        tile_k = msettilek(k - kk);
        mint8_t tr0 = mla_m(A + i * k + kk, lda_bytes);
        mint8_t tr1 = mlb_m(B + kk * n + j, ldb_bytes);
        acc = mqma_mm(acc, tr0, tr1);
      }
      msc_m(acc, C + i * n + j, ldc_bytes);
    }
  }
  return 0;
}

static inline int matmul(A, B,C,
                         int M, int N, int K){
  return matmul_batch1(A, B,C,M,N,K);
}


    return 0;
}
