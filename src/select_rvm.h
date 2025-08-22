#ifndef __SELECT_RVM_H__
#define __SELECT_RVM_H__

#include "../include/matrix/matrix_intrinsic.h"
#include "tensor.h"
#include <riscv_matrix.h>
#include <stddef.h>

// Only Matrix-0.5
#define CONFIGURATION_MTYPE(hi, li)                                            \
  do {                                                                         \
    msettypei((li));                                                           \
    msettypehi((hi));                                                          \
  } while (0);

// SET DTYPE=FP16 SEW=16
#define SET_MBA0_FP16() CONFIGURATION_MTYPE(0x1, 0x1)
// SET DTYPE=FP32 SEW=32
#define SET_MBA0_FP32() CONFIGURATION_MTYPE(0x4, 0x2)
// SET DTYPE=FP16,F32 SEW=16
#define SET_MBA0_FP16_FP32() CONFIGURATION_MTYPE(0x5, 0x1)
// SET DTYPE=FP16,FP32 SEW=32
#define SET_MBA0_FP32_FP16() CONFIGURATION_MTYPE(0x5, 0x2)


// 查表函数实现
static inline int select_rvm(uint16_t index, Tensor *dst) {
    const float32_t der_1[] = {2.9103830456733704e-11,
      5.820766091346741e-11,
        1.1641532182693481e-10,
        2.3283064365386963e-10,
        4.656612873077393e-10,
        9.313225746154785e-10,
        1.862645149230957e-09,
        3.725290298461914e-09,
        7.450580596923828e-09,
        1.4901161193847656e-08,
        2.9802322387695312e-08,
        5.960464477539063e-08,
        1.1920928955078125e-07,
        2.384185791015625e-07,
        4.76837158203125e-07,
        9.5367431640625e-07,
        1.9073486328125e-06,
        3.814697265625e-06,
        7.62939453125e-06,
        1.52587890625e-05,
        3.0517578125e-05,
        6.103515625e-05,
        0.0001220703125,
        0.000244140625,
        0.00048828125,
        0.0009765625,
        0.001953125,
        0.00390625,
        0.0078125,
        0.015625,
        0.03125,
        0.0625,
        0.125,
        0.25,
        0.5,
        1.0,
        2.0,
        4.0,
        8.0,
        16.0,
        32.0,
        64.0,
        128.0,
        256.0,
        512.0,
        1024.0,
        2048.0,
        4096.0,
        8192.0,
        16384.0,
        32768.0,
        65536.0};
  
    float16_t *pdst = (float16_t *)dst->data;

    // 1. 查表获取标量值
    float32_t scalar_value = der_1[index];

    const float16_t ans[8*8] = {
      -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105,
      -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105,
      -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105,
      -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105,
      -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105,
      -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105,
      -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105,
      -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105, -5.105};
    // 2. 矩阵参数设置
    int M = 8;
    int N = 8;
    const size_t target_row = 0;  // 第1行（从0开始）
    const size_t target_col = 0;  // 第1列
    const size_t stride = N * sizeof(float16_t);
    
    // 3. 设置矩阵环境
    SET_MBA0_FP16();          // 设置矩阵加速器为FP16模式
    msettilem(M);            // 设置行数为8
    // msettilen(M);  
    msettilen(N);            // 设置列数为8
    
    // 4. 创建空矩阵并移动标量到指定位置
    const size_t position = target_col << 16 | target_row; // 位置编码
    mfloat16_t acc0 = mlc_m(ans,stride);
    acc0 = mfsub_mm(acc0, acc0);
    SET_MBA0_FP16_FP32();
    mfloat32_t acc1 = mfwcvt_fw_f_m(acc0);
  
    mfloat32_t target_matrix = mfmv_a_f(acc1, scalar_value, position);
    
    // 5. 广播元素到整个矩阵
    mfloat32_t result_matrix = mbcce_m(target_matrix);
    SET_MBA0_FP32_FP16();
    mfloat16_t md = mfncvt_f_fw_m(result_matrix);
    SET_MBA0_FP16();
    // 6. 存储结果到输出缓冲区
    msc_m(md, pdst, stride);

    return 0;
}

#endif