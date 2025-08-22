#ifndef MLA_H_
#define MLA_H_


#include "../include/matrix/matrix_intrinsic.h"
#include "tensor.h"
#include <riscv_matrix.h>
#include <stddef.h>


#include "mla_qcmpt.h"
#include "mla_kvcmpt.h"
#include "mla_attention.h"


// 结果指定到.scdata.output存储区域
// uint8_t dstData[OUT_SIZE * sizeof(float16_t)] __attribute__((__section__(".scdata.output")));

// 输入h w result
int mla_cmpt(Tensor *result, Tensor *ht ,Tensor *wdq ,Tensor *wuq ,Tensor *wqr ,
    Tensor *wdkv, Tensor *wuk, Tensor *wuv,Tensor *wkr ,
    Tensor *wo ){

        tensor_new_2d(qtMat,nh*(dh+dhr),L,sizeof(float16_t),NULL) ;
        cmpt_q(&qtMat, ht, wdq, wuq, wqr);

        // tensor_new_2d(kMat,nh*(dh+dhr),L,sizeof(float16_t),NULL) ;
        // tensor_new_2d(vMat,nh*dh,L,sizeof(float16_t),NULL) ;
        // cmpt_kv(&kMat, &vMat, ht, wdkv, wuk, wuv,wkr) ;
        
        // tensor_new_2d(scoreMat,nh*dh,L,sizeof(float16_t),NULL) ;
        // cmpt_attention(&qtMat,&kMat,&vMat,&scoreMat) ;

        // matmul(result,wo,&scoreMat) ;

    return 0 ;
}

#endif