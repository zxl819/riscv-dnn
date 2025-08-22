#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../../src/perf.h"
#include "/nfs/home/zhengsihan/Code/new-tests/src-tests/riscv-dnn/include/incbin.h"
#include "./params.h"
#include "./cpi.h"

#include "/nfs/home/zhengsihan/Code/new-tests/src-tests/riscv-dnn/include/matrix/matrix_intrinsic.h"
#include "/nfs/home/zhengsihan/Code/new-tests/src-tests/riscv-dnn/src/tensor.h"
#include <riscv_matrix.h>
#include <stddef.h>


// 读取.bin输入数据，嵌入data段
INCBIN(htData, "ht.bin", ".scdata.params"); 
INCBIN(wdqData, "wdq.bin", ".scdata.params"); 
INCBIN(wuqData, "wuq.bin", ".scdata.params"); 
INCBIN(wqrData, "wqr.bin", ".scdata.params"); 
INCBIN(wdkvData, "wdkv.bin", ".scdata.params"); 
INCBIN(wukData, "wuk.bin", ".scdata.params"); 
INCBIN(wuvData, "wuv.bin", ".scdata.params"); 
INCBIN(wkrData, "wkr.bin", ".scdata.params"); 
INCBIN(woData, "wo.bin", ".scdata.params"); 

// 放在.scdata段的全局变量、静态变量和大矩阵大数组
//Q
uint8_t qtMat_data[nh*(dh+dhr) * L * sizeof(float16_t)]
    __attribute__((__section__(".scdata.output")));
uint8_t qcMat_2d_data[nh*dh * L * sizeof(float16_t)]
    __attribute__((__section__(".scdata.output")));
uint8_t qrMat_2d_data[nh*dhr * L * sizeof(float16_t)]
    __attribute__((__section__(".scdata.output")));
uint8_t cqMat_data[dcq *L *  sizeof(float16_t)];
    __attribute__((__section__(".scdata.output")));
//KV
uint8_t kMat_data[nh*(dh+dhr) *L *  sizeof(float16_t)];
    __attribute__((__section__(".scdata.output")));
uint8_t vMat_data[nh*dh *L *  sizeof(float16_t)];
    __attribute__((__section__(".scdata.output")));
uint8_t ckvMat_data[dckv *L *  sizeof(float16_t)];
    __attribute__((__section__(".scdata.output")));
uint8_t kcMat_data[nh*dh *L *  sizeof(float16_t)];
    __attribute__((__section__(".scdata.output")));  
uint8_t krMat_data[dhr *L *  sizeof(float16_t)];
    __attribute__((__section__(".scdata.output"))); 
// attention
uint8_t scoreMat_data[nh*dh *L *  sizeof(float16_t)];
    __attribute__((__section__(".scdata.output"))); 
uint8_t qtk_data[L *L *  sizeof(float16_t)];
    __attribute__((__section__(".scdata.output"))); 
uint8_t softkv_data[L *L *  sizeof(float16_t)];
    __attribute__((__section__(".scdata.output"))); 
uint8_t ot_data[nh*dh *L *  sizeof(float16_t)];
    __attribute__((__section__(".scdata.output"))); 

// 保存计算结果
uint8_t dstData[OUT_SIZE * sizeof(float16_t)] 
__attribute__((__section__(".scdata.output")));

// 临时数组
// uint8_t cqMat_tmp_data[dcq *L *  sizeof(float16_t)];


#include "/nfs/home/zhengsihan/Code/new-tests/src-tests/riscv-dnn/src/mla_qcmpt.h"
#include "/nfs/home/zhengsihan/Code/new-tests/src-tests/riscv-dnn/src/mla_kvcmpt.h"
// #include "/home/wsl/Lab/new-tests/src-tests/ops/src/mla_attention.h"
#include "/nfs/home/zhengsihan/Code/new-tests/src-tests/riscv-dnn/src/softmax.h"


int main(int argc, char **argv){
    printf("Begin\n");

    const int nd    =d ;
    const int nL    = L ;
    const int ndcq  = dcq ;
    const int nnh   = nh ;
    const int ndh   = dh ;
    // const int qclength = nh*dh;
    const int ndhr  = dhr ;
    const int ndckv = dckv ;

    tensor_new_2d(dstData,nd,nL,sizeof(float16_t), &dstData) ;  // output

    tensor_new_2d(htMat,  nd,nL,sizeof(float16_t),htData ) ;
    // static float16_t htMat[nd][nL]  __attribute__((__section__(".scdata.output"))) ;
    tensor_new_2d(wdqMat ,ndcq , nd,sizeof(float16_t), wdqData) ;    
    tensor_new_2d(wuqMat ,nnh*ndh , ndcq ,sizeof(float16_t), wuqData) ;
    tensor_new_2d(wqrMat ,nnh*ndhr , ndcq,sizeof(float16_t), wqrData) ;
    tensor_new_2d(wdkvMat ,ndckv ,nd ,sizeof(float16_t), wdkvData) ;
    tensor_new_2d(wukMat , nnh*ndh,ndckv ,sizeof(float16_t), wukData) ;
    tensor_new_2d(wuvMat ,nnh*ndh ,ndckv ,sizeof(float16_t), wuvData) ;
    tensor_new_2d(wkrMat ,ndhr ,nd ,sizeof(float16_t), wkrData) ;
    tensor_new_2d(woMat ,nd , nnh*ndh ,sizeof(float16_t), woData) ;
    
    stats( 
        mla_cmpt(&dstData, &htMat ,&wdqMat ,&wuqMat ,&wqrMat ,
            &wdkvMat, &wukMat, &wuvMat,&wkrMat ,
            &woMat ) ;
    ,1) ;

    printf("End\n");

    return 0;
}

int mla_cmpt(Tensor *result, Tensor *ht ,Tensor *wdq ,Tensor *wuq ,Tensor *wqr ,
    Tensor *wdkv, Tensor *wuk, Tensor *wuv,Tensor *wkr ,
    Tensor *wo ){

       
        tensor_new_2d(qtMat,L,nh*(dh+dhr),sizeof(float16_t),qtMat_data) ;
        cmpt_q(&qtMat, ht, wdq, wuq, wqr);

        tensor_new_2d(kMat,nh*(dh+dhr),L,sizeof(float16_t),kMat_data) ;
        tensor_new_2d(vMat,nh*dh,L,sizeof(float16_t),vMat_data) ;
        cmpt_kv(&kMat, &vMat, ht, wdkv, wuk, wuv,wkr) ;
        
        
        // cmpt_attention(&qtMat,&kMat,&vMat,&scoreMat) ;
        // qt*k
        tensor_new_2d(qtk,L,L,sizeof(float16_t),qtk_data)
        matmul(&qtk,&qtMat,&kMat) ;
        // softmax
        tensor_new_2d(softkv,L,L,sizeof(float16_t),softkv_data) ;
        softmax(&softkv,&qtk) ;

        tensor_new_2d(otMat,nh*dh,L,sizeof(float16_t),ot_data) ;
        matmul(&otMat,&vMat,&softkv) ;

        matmul(result,wo,&otMat) ;

    return 0 ;
}

