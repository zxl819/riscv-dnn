#include <stdio.h>
#include <stdlib.h>
#include <riscv_matrix.h>
// // #include <stdint.h>

// #include "../../../src/matmul_tile.h"
// #include "../../../src/perf.h"
// #include "../../../include/incbin.h"

// #include "./cpi.h"

// #include "params.h"

// INCBIN(src1Data, "src1.bin", ".scdata.params");   // src1.bin数据嵌入到src1Data
// INCBIN(src2Data, "src2.bin", ".scdata.params");   // src2.bin数据嵌入到src2Data



// // 定义一个目标矩阵数组，并把它指定到.scdata.output存储区域
// uint8_t dstData[OUT_SIZE * sizeof(float16_t)] __attribute__((__section__(".scdata.output")));

//int main(int argc, char **argv)
int main()
{

    int m = 100;
    int n = 100;
    int k = 100;

    int tile_m = 0, tile_n = 0, tile_k = 0;
    for (int i = 0; i < m; i += tile_m) {
        tile_m = msettilem(m - i);
        for (int j = 0; j < n; j += tile_n) {
            tile_n = msettilen(n - j);
            for (int kk = 0; kk < k; kk += tile_k) {
                tile_k = msettilek(k - kk);
            }
        }
    }
    return 0;
}
