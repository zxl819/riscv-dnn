#include <stdio.h>
#include <stdlib.h>
#include <riscv_matrix.h>
#include <stddef.h>

#define DIM_M 8
#define DIM_K 8
#define DIM_N 8

/* 其中.matA,B,C由link.ld中定义
 mat data address 紧密排布 
  A：0x80040000 ~ 0x80040040（64 bytes）。
  B：0x80040040 ~ 0x80040080（64 bytes）。
  C：0x80040080 ~ 0x80040180（256 bytes）。
. = 0x80040000;起始地址
. = ALIGN(64); _matA_start = .; .matA : { *(.matA) } _matA_end = .; Address 64 bytes对齐
. = ALIGN(64); _matB_start = .; .matB : { *(.matB) } _matB_end = .; Address 64 bytes对齐
. = ALIGN(64); _matC_start = .; .matC : { *(.matC) } _matC_end = .;*/

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

  const int dataSize = sizeof(int8_t);
  // matmul
  int tile_m = 0, tile_n = 0, tile_k = 0;

  for (int i = 0; i < m; i += tile_m) {
    tile_m = msettilem(m - i);
    for (int j = 0; j < n; j += tile_n) {
      tile_n = msettilen(n - j);
      //initialize acc using C
      //mint32_t acc=mlc_m(C + i * n + j, ldc_bytes);
      mint32_t acc;
      for (int kk = 0; kk < k; kk += tile_k) {
        tile_k = msettilek(k - kk);
        mint8_t tr0 = mla_m(A + i * k + kk, lda_bytes);
        mint8_t tr1 = mlb_m(B + kk * n + j, ldb_bytes);
        acc = mqma_mm(acc, tr0, tr1);
      }
      // msc_m(acc, C + i * n + j, ldc_bytes);
    }
  }
  return 0;
}

static inline int matmul(const int8_t *A, const int8_t *B, int32_t *C,
                          int M, int N, int K) {
  return matmul_batch1(A, B,C,M,N,K);
}

int main()
{
    matmul(A, B, C, DIM_M, DIM_N, DIM_K);
    return 0;

}
