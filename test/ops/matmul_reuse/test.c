#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <riscv_matrix.h>
#include <stddef.h>
#include "data.h"
#include <string.h>
#include "matrix/matrix_intrinsic.h"

#define DIM_M 16
#define DIM_K 16
#define DIM_N 16

/* 其中.matA,B,C由link.ld中定义
    . = 0x80040000; _matA_start = .; .matA : { *(.matA) } _matA_end = .;
    . = 0x80050000; _matB_start = .; .matB : { *(.matB) } _matB_end = .;
    . = 0x80060000; _matC_start = .; .matC : { *(.matC) } _matC_end = .;*/

// 8x8x8 int8_t matmul，result in int32_t
__attribute__((section(".matA"), aligned(64))) int8_t  A[DIM_M*DIM_K];
__attribute__((section(".matB"), aligned(64))) int8_t  B[DIM_K*DIM_N];
__attribute__((section(".matC"), aligned(64))) int32_t C[DIM_M*DIM_N];

static inline int matmul_batch1(const int8_t *A, const int8_t *B, int32_t *C,
                                 int m, int n, int k) {
  // lda, ldb, ldc in bytes for msetilem/len/k                                
  const int lda_bytes = k * (int)sizeof(int8_t);
  const int ldb_bytes = n * (int)sizeof(int8_t);
  const int ldc_bytes = n * (int)sizeof(int32_t);
//   const int lda_bytes = 64 * (int)sizeof(int8_t);
//   const int ldb_bytes = 64 * (int)sizeof(int8_t);
//   const int ldc_bytes = 16 * (int)sizeof(int32_t);

  const int dataSize = sizeof(int8_t);
  // matmul
  int tile_m = 0, tile_n = 0, tile_k = 0;

  for (int i = 0; i < m; i += tile_m) {
    tile_m = msettilem(m - i);
    //printf("i = %d\n", i);  // 添加调试
    for (int j = 0; j < n; j += tile_n) {
      tile_n = msettilen(n - j);
      SET_MBA0_I32();
      //mint32_t acc1;
      //initialize acc using C
      mint32_t acc1 = mlc_m(C + i * n + j, ldc_bytes);
      for (int kk = 0; kk < k; kk += tile_k) {
        tile_k = msettilek(k - kk);
        mint8_t tr0 = mla_m(A + i * k + kk, lda_bytes);
        mint8_t tr1 = mlb_m(B + kk * n + j, ldb_bytes);
        //acc = mqma_mm(acc, tr0, tr1);
        acc1 = mqma_b_mm(acc1, tr0, tr1);
      }
      msc_m(acc1, C + i * n + j, ldc_bytes);
    }
  }
  return 0;
}

static inline int matmul(const int8_t *A, const int8_t *B, int32_t *C,
                          int m, int n,int k) {
  return matmul_batch1(A, B, C, m, n, k);
}

int main()
{
    // 将 data.h 中的 A_i8/B_i8 拷贝到固定段 .matA/.matB；C 清零
    memcpy(A, A_i8, M * K * sizeof(int8_t));
    memcpy(B, B_i8, K * N * sizeof(int8_t));
    memset(C, 0, M * N * sizeof(int32_t));

    // 计算
    matmul(A, B, C, DIM_M, DIM_N, DIM_K);
    printf("C result:\n");
    for (int i = 0; i < DIM_M; i++) {
        for (int j = 0; j < DIM_N; j++) {
            printf("%d ", C[i * DIM_N + j]);
        }
        printf("\n");
    
      }
    // 与 golden (D_i32) 对比
    int mismatch = 0;
    for (int idx = 0; idx < DIM_M * DIM_N; ++idx) {
        if (C[idx] != D_i32[idx]) {
            if (mismatch < 16) { // 只打印前16个差异
                int r = idx / DIM_N;
                int c = idx % DIM_N;
                printf("mismatch (%d,%d): got %d expected %d\n",
                       r, c, C[idx], D_i32[idx]);
            }
            mismatch++;
        }
    }
    if (mismatch == 0) {
        printf("Compare with D: PASS\n");
    } else {
        printf("Compare with D: FAIL, total mismatches=%d\n", mismatch);
    }



    return 0;
}
