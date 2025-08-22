#ifndef __MATMUL_RVM_H__
#define __MATMUL_RVM_H__

#include "../include/matrix/matrix_intrinsic.h"
#include "tensor.h"
#include <riscv_matrix.h>
#include <stddef.h>

// #define FP16_ACC16 1

static inline int matmul_batch1(Tensor *dst, Tensor *src1, Tensor *src2) {
  int m = src1->shape[0];
  int k = src1->shape[1];

  int h2 = src2->shape[0];
  int n = src2->shape[1];

  assert(k == h2);

  int hout = dst->shape[0];
  int wout = dst->shape[1];

  assert(hout == m && wout == n);

  float16_t *psrc1 = (float16_t *)src1->data;
  float16_t *psrc2 = (float16_t *)src2->data;
  float16_t *pdst = (float16_t *)dst->data;

  const int dataSize = sizeof(float16_t);
  int tile_m = 0, tile_n = 0, tile_k = 0;

  for (int i = 0; i < m; i += tile_m) {
    tile_m = msettilem(m - i);
    for (int j = 0; j < n; j += tile_n) {
      tile_n = msettilen(n - j);
      for (int kk = 0; kk < k; kk += tile_k) {
        tile_k = msettilek(k - kk);
      }
    }
  }
  return 0;
}

static inline int matmul(Tensor *dst, Tensor *src1, Tensor *src2) {
  return matmul_batch1(dst, src1, src2);
}

#endif // __MATMUL_RVM_H__
