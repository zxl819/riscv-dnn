#ifndef __CONV_BN_RELU_H__
#define __CONV_BN_RELU_H__

#include "../include/matrix/matrix_intrinsic.h"
#include "mme.h"
#include "tensor.h"
#include <riscv_matrix.h>
#include <stddef.h>

static inline int batchnorm(Tensor *dst, Tensor *src,Tensor *alpha, Tensor *beta) {
 
  


  int dataSize = sizeof(float16_t);
  float16_t *psrc1 = (float16_t *)src->data;
 
  float16_t *pdst = (float16_t *)dst->data;
  float16_t *palpha = (float16_t *)alpha->data;
  float16_t *pbeta = (float16_t *)beta->data;

  int m = src->shape[0];
  int n = src->shape[1];
  int cin = src->shape[2];

  int stride_d = 0 ;

  int tilem, tilen, tilek;
 

  // conv
  for (int i = 0; i < m; i += tilem) {
    msettypei(0x1);
    msettypehi(0x1);
    tilem = msettilem(m - i);


    for (int j = 0; j < n; j += tilen) {
      msettypei(0x1);
      msettypehi(0x1);
      tilen = msettilen(n - j);
      mfloat16_t mbeta = mlc_m(pbeta + j, stride_d);
      mfloat16_t malpha = mlc_m(palpha + j, stride_d);
      mfloat16_t zero;
      zero = mfsub_mm(zero, zero);
      msettypei(0x2);
      msettypehi(0x4);
      mfloat32_t acc0;
      acc0 = mfsub_f_mm(acc0, acc0);
      
          for (int skc = 0; skc < cin; skc += tilek) {
            msettypei(0x1);
            msettypehi(0x1);
            tilek = msettilek(cin - skc);
            
            SET_MBA0_FP16_FP32();

            
            
          }
        }
      }
      SET_MBA0_FP32_FP16();
      mfloat16_t acc1 = mfncvt_f_fw_m(acc0);
      // batchnormal
      msettypei(0x1);
      msettypehi(0x1);

      mbeta = mbccr_m(mbeta);
      malpha = mbccr_m(malpha);
      acc1 = mfmul_mm(acc1, malpha);
      acc1 = mfadd_mm(acc1, mbeta);

      // relu
      acc1 = mfmax_mm(acc1, zero);
      msc_m(acc1, pdst + i * stride_d / dataSize + j, stride_d);
    }
  }

  return 0;



}

#endif //__CONV_BN_RELU_H__