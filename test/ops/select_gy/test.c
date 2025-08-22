#include <stdio.h>
#include <stdlib.h>

// #include <stdint.h>

#include "../../../src/select_rvm.h"
#include "../../../src/perf.h"
#include "../../../include/incbin.h"

#include "./cpi.h"

#include "params.h"

uint8_t dstData[8 * 8 * sizeof(float16_t)]
    __attribute__((__section__(".scdata.output")));


// void print_matrix(float16_t *matrix, int rows, int cols) {
//     for (int i = 0; i < rows; i++) {
//         for (int j = 0; j < cols; j++) {
//             printf("%f ", matrix[i * cols + j]);
//         }
//         printf("\n");
//     }
// }

int main(int argc, char **argv)
{
    printf("Begin\n");
    
    const uint16_t index = INDEX;


    //if (DEBUG_PRINT) {
    //    printf("In Shape:\n\t(m, k, n) = (%d, %d, %d)\n",
    //                m, k, n);
    //}

    // 
    tensor_new_2d(dst, 8, 8, sizeof(float16_t),&dstData); 
    // PERF_BEGIN();  // perf计算开始
    stats(
        for (int i = 0; i < NLOOPS; i++) {
            select_rvm(index,&dst);
        }
    , 1) ;

    // PERF_END();   // perf计算结束
    
    printf("End\n");

    return 0;
}
