#ifndef MLA_KVCMPT_H_
#define MLA_KVCMPT_H_

#include "../include/matrix/matrix_intrinsic.h"
#include "tensor.h"
#include <riscv_matrix.h>
#include <stddef.h>
// #include <math.h>

#include "./mla_sincos.h"

#include "matmul.h"
#include "./../test/ops/mla/params.h"

static float16_t krt[L][dhr] __attribute__((section(".scdata.output")));
static float16_t arrkr[L][dhr] __attribute__((section(".scdata.output")));
static float16_t arrkc[nh*dh][L] __attribute__((section(".scdata.output"))) ;
static float16_t arrcon[nh*(dh+dhr)][L] __attribute__((section(".scdata.output"))) ;

static float16_t arrcos2[dhr] __attribute__((section(".scdata.output")))  ;
static float16_t arrsin2[dhr] __attribute__((section(".scdata.output")))  ;
static float16_t arr22[dhr] __attribute__((section(".scdata.output")));
static float16_t arr12[dhr] __attribute__((section(".scdata.output"))) ;

static inline int cmpt_kv(Tensor *dstk,Tensor *dstv,Tensor *ht, Tensor *wdkv, Tensor *wuk, Tensor *wuv,Tensor *wkr){

        
    tensor_new_2d(ckvMat,dckv,L , sizeof(float16_t),ckvMat_data);
    matmul(&ckvMat,wdkv,ht);

    //kc = wuk * ckv
   
    tensor_new_2d(kcMat,nh*dh,L,sizeof(float16_t),kcMat_data) ;
    matmul(&kcMat,wuk,&ckvMat) ;

    // // vc = wuv * ckv
   
    matmul(dstv,wuv,&ckvMat) ;

    //rope    
    // int dhr = 64 ;
    tensor_new_2d(krMat,dhr,L,sizeof(float16_t),krMat_data) ;
    matmul(&krMat,wkr,ht) ;

    // col1 col2 cos sin
    // 读出kr数据
    float16_t *ptr_kr = (float16_t *)krMat.data ;
    // float16_t krt[text_length][dhr];
    for (int i = 0 ; i < dhr ; i++)
    {
        for(int j=0 ; j<L ; j++){
            krt[j][i]=*ptr_kr ;
            ptr_kr++;
        }
    }

    // float16_t arrkr[L][dhr]={0} ;
    // float16_t arrcon[nh*(dh+dhr)][L] = {0} ;
    // 每一行rope编码
    for(int t=0; t<L ;t++){
        // float16_t arr1[dhr] ;
        //col1
        for (int ai=0 ; ai<dhr ; ai++){
            arr12[ai] = krt[t][ai] ;
        }
        //col2
        // float16_t arr2[dhr] ;
        for(int bi=0 ; bi<(dhr) ; bi=bi+2){
            arr22[bi]=( 0 - arr12[bi+1]);
            arr22[bi+1] = arr12[bi] ;
        }
        //sin cos
        // float16_t arrcos[dhr]  ;
        // float16_t arrsin[dhr]  ;
        for( int ci=0 ; ci<(dhr) ; ci=ci+2){
          float angle = t*(ci/2) ;
          arrcos2[ci] = my_cos(angle) ;
          arrcos2[ci+1] = my_cos(angle);
          arrsin2[ci] = my_sin(angle) ;
          arrsin2[ci+1] = my_sin(angle) ;
        }
        // rope结果计算
        for (int ri = 0; ri < dhr; ri++)
        {
            arrkr[t][ri] = arr12[ri]*arrcos2[ri] + arr22[ri]*arrsin2[ri] ;
        }
        // 拼接k内容和编码
        // 读出k矩阵内容
        // float16_t arrkc[nh*dh][L] = {0} ;
        float16_t *ptr_kc = (float16_t *)kcMat.data ;
        for(int row1=0 ; row1<L ; row1++){
            for(int colm1=0 ; colm1<(nh*dh) ; colm1++ ){
              arrkc[row1][colm1] = *ptr_kc ;
              ptr_kc++ ;
            }
        }
        //拼接
        
        for(int ni=0 ; ni<L ; ni++){
            for (int nj=0 ; nj<(nh*(dh+dhr)); nj++){
              if (nj < (nh*dh))
                arrcon[nj][ni] = arrkc[nj][ni] ;
              else
                arrcon[nj][ni] = arrkr[ni][nj-(nh*dh)] ;
            }
        }  
    }

    //输出
    dstk->data = arrcon ;    
    // dstk->elemsize = sizeof(float16_t) ;
    
    return 0 ;
}



#endif