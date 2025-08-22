#ifndef MLA_QCMPT_H_
#define MLA_QCMPT_H_


#include "../include/matrix/matrix_intrinsic.h"
#include "tensor.h"
#include <riscv_matrix.h>
#include <stddef.h>
// #include <math.h>

#include "./mla_sincos.h"

#include "./matmul.h"
#include "./../test/ops/mla/params.h"

// input  ht // wdq wuq  
// Q : matmul ht*wdq=cq , matmul wuq*cq=qc , rope: cq  ,concat

static float16_t cqt[L][nh*dhr] __attribute__((section(".scdata.output")));
// static float16_t arr1[nh*dhr] __attribute__((section(".scdata.output"))) ;
// static float16_t arr2[nh*dhr] __attribute__((section(".scdata.output"))) ;
// static float16_t arrcos[nh*dhr] __attribute__((section(".scdata.output"))) ;
// static float16_t arrsin[nh*dhr] __attribute__((section(".scdata.output"))) ;
static float16_t arrqr[L][nh*dhr] __attribute__((section(".scdata.output"))) ;
static float16_t arrqc[nh*dh][L] __attribute__((section(".scdata.output")))  ;  
static float16_t arrcon[nh*(dh+dhr)][L]  __attribute__((section(".scdata.output"))) ;
static float16_t arrcont[L][nh*(dh+dhr)] __attribute__((section(".scdata.output"))) ;

static float16_t arr1[nh*dhr] __attribute__((section(".scdata.output"))) ;
static float16_t arr2[nh*dhr] __attribute__((section(".scdata.output"))) ;
static float16_t arrcos[nh*dhr] __attribute__((section(".scdata.output")))  ;
static float16_t arrsin[nh*dhr]  __attribute__((section(".scdata.output")))  ;

// 这里wuq使用2d输入
static inline int cmpt_q(Tensor *dstq ,Tensor *ht ,Tensor *wdq ,Tensor *wuq ,Tensor *wqr ){
  
   int text_length = L ;
   int dcq_length = dcq ;
  // wdq*ht=cq (1563,length) ，wdq是二维的，
  // float16_t **cqMat[dcq_length][text_length];
  tensor_new_2d(cqMat, dcq_length , L , sizeof(float16_t), cqMat_data);
  matmul(&cqMat ,wdq ,ht) ;

  // wuq*cq=qc  计算q的内容部分  
  tensor_new_2d(qcMat_2d, nh*dh, L, sizeof(float16_t), qcMat_2d_data);
  // int qclength=nh*dh;
  // float16_t ** qcMat_2d[qclength][text_length] ;
  matmul(&qcMat_2d, wuq , &cqMat );
  
  // 计算q的编码部分  qrt = wqr * cq
  // int dhr = 64 ;
  tensor_new_2d(qrMat,nh*dhr,L,sizeof(float16_t), qrMat_2d_data);
  // int qrlength=nh*dhr ;
  // float16_t ** qrMat[qrlength][text_length];

  matmul(&qrMat,wqr,&cqMat) ;

  float16_t *ptr_cqr = (float16_t *)qrMat.data ;  //cq矩阵data段起始位置
  // float16_t cqt[L][nh*dhr];  // 转置后的存储矩阵cqt   按行读出按列写入
  for(int colm=0 ; colm<(nh*dhr) ; colm++ ){
    for(int row=0 ; row<text_length ; row++){
      cqt[row][colm] = *ptr_cqr ;
      ptr_cqr++ ;
    }
  }


    // float16_t arr1[nh*dhr]  ;

  for(int t=0 ; t<L; t++) {
    //计算col1的值
    // float16_t arr1[nh*dhr]  ;
    for (int ai=0 ; ai<(nh*dhr) ; ai++){
        arr1[ai] =  cqt[t][ai] ;       
    }
    //计算col2的值 预处理
    // float16_t arr2[nh*dhr]  ;
    for(int bi=0 ; bi<(nh*dhr) ; bi=bi+2){
      arr2[bi]=( 0 - arr1[bi+1]);
      arr2[bi+1] = arr1[bi] ;
    }
    // 计算cos sin
    // float16_t arrcos[nh*dhr] ;
    // float16_t arrsin[nh*dhr] ;
    for( int ci=0 ; ci<(nh*dhr) ; ci=ci+2){
      float angle = t*(ci/2) ;
      arrcos[ci] = my_cos(angle) ;
      arrcos[ci+1] = my_cos(angle);
      arrsin[ci] = my_sin(angle) ;
      arrsin[ci+1] = my_sin(angle) ;
    }
    // 计算rope编码的结果
    // float16_t rope_result[nh*dhr] = {0} ;
    for(int ri=0 ; ri<(nh*dhr) ; ri++){
      // rope_result[ri] = arr1[ri]*arrcos[ri] + arr2[ri]*arrsin[ri] ;
      arrqr[t][ri] = arr1[ri]*arrcos[ri] + arr2[ri]*arrsin[ri] ;
    } 
  }
  // 拼接Q内容和编码
  // float16_t arrqc[nh*dh][L]  ;   // 读出q内容矩阵
  float16_t *ptr_qc = (float16_t *)qcMat_2d.data ;
  for(int row1=0 ; row1<text_length ; row1++){
    for(int colm1=0 ; colm1<(nh*dh) ; colm1++ ){
      arrqc[row1][colm1] = *ptr_qc ;
      ptr_qc++ ;
    }
  }
  //拼接
  int scale=1 ;
  // float16_t arrcon[nh*(dh+dhr)][L] = {0} ;
  // float16_t arrcont[L][nh*(dh+dhr)] = {0} ; // transpose
  for(int ni=0 ; ni<text_length ; ni++){
    for (int nj=0 ; nj<(nh*(dh+dhr)); nj++){
      if (nj < (nh*dh)){
        arrcon[nj][ni] = arrqc[nj][ni] ;
        arrcont[ni][nj]=arrqc[nj][ni] ;
      }       
      else {
        arrcon[nj][ni] = arrqr[ni][nj-(nh*dh)] ;
        arrcont[ni][nj] = arrqr[ni][nj-(nh*dh)] * scale;  //直接加入attention的缩放
      }        
    }
  }

//  tensor_new_2d(qMat,nh*(dh+dhr),L,sizeof(float16_t), arrcont);
tensor_new_2d(qMat,L,nh*(dh+dhr),sizeof(float16_t), arrcont);
  // 这里直接存QT，

  return 0 ;
}

#endif