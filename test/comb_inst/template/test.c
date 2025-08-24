#include <stdio.h>
#include <stdlib.h>

// #include <stdint.h>

#include "../../../src/matmul_tile.h"
#include "../../../src/perf.h"
#include "../../../include/incbin.h"

#include "./cpi.h"

#include "params.h"

INCBIN(src1Data, "src1.bin", ".scdata.params");   // src1.bin数据嵌入到src1Data
INCBIN(src2Data, "src2.bin", ".scdata.params");   // src2.bin数据嵌入到src2Data



// 定义一个目标矩阵数组，并把它指定到.scdata.output存储区域
uint8_t dstData[OUT_SIZE * sizeof(float16_t)] __attribute__((__section__(".scdata.output")));

int main(int argc, char **argv)
{

    const int m = 100;
    const int k = 100;
    const int n = 100;



    // 创建源矩阵、目标矩阵，初始化矩阵数据
    tensor_new_2d(src1Mat, m, k, sizeof(float16_t), src1Data);  // src1data初始化src1Mat矩阵
    tensor_new_2d(src2Mat, k, n, sizeof(float16_t), src2Data);
    tensor_new_2d(dstMat, m, n, sizeof(float16_t), &dstData);

    // PERF_BEGIN();  // perf计算开始
    stats(
        for (int i = 0; i < NLOOPS; i++) {
            matmul(&dstMat, &src1Mat, &src2Mat);
        }
    , 1) ;


    return 0;
}
