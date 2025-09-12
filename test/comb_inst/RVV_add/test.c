
#include <riscv_vector.h>
#include <stdio.h>

int main() {
    
    // 定义向量长度
    size_t vl = 8;  // 向量长度，根据硬件和RVV版本调整

    // 创建两个向量
    vint32m1_t a, b, result;
    
    // 初始化向量数据
    int32_t data_a[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int32_t data_b[8] = {8, 7, 6, 5, 4, 3, 2, 1};
    
    // 将数据加载到向量中
    a = __riscv_vle32_v_i32m1(data_a, vl);
    b = __riscv_vle32_v_i32m1(data_b, vl);
    
    // 执行加法
    result = __riscv_vadd_vv_i32m1(a, b, vl);
    
    // 输出结果
    int32_t result_data[8];
    __riscv_vse32_v_i32m1(result_data, result, vl);
    // printf("Result: ");
    // for (int i = 0; i < 8; i++) {
    //     printf("%d ", result_data[i]);
    // }
    // printf("\n");

    return 0;
}
