#ifndef __MATMUL_RVM_H__
#define __MATMUL_RVM_H__

#include "../include/matrix/matrix_intrinsic.h"
#include "tensor.h"
#include <riscv_matrix.h>
#include <stddef.h>
#include <assert.h>


static inline int matmul_batch1(int A,
                        int B,
                        int C,
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

static inline int matmul(int A,
                         int B,
                         int C,
                         int M, int N, int K){
  return matmul_batch1(A,B,C,M,N,K);
}

#endif // __MATMUL_RVM_H__
