#ifndef MLA_ATTENTION_H_
#define MLA_ATTENTION_H_

#include "../include/matrix/matrix_intrinsic.h"
#include "tensor.h"
#include <riscv_matrix.h>
#include <stddef.h>
// #include <math.h>

#include "mla_kvcmpt.h"
#include "mla_qcmpt.h"
#include "matmul.h"
#include "softmax.h"
#include "./../test/ops/mla/params.h"

static inline int cmpt_attention(Tensor *result, Tensor *ht ,Tensor *wdq ,Tensor *wuq ,Tensor *wqr ,
    Tensor *wdkv, Tensor *wuk, Tensor *wuv,Tensor *wkr ,
    Tensor *wo ){

    int text_length = L ;
    tensor_new_2d(cqMat, 1536 , text_length , sizeof(float16_t), NULL);
    matmul(&cqMat ,wdq ,ht) ;

    return 0 ;
}

#endif