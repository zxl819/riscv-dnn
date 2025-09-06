#include <riscv_vector.h>
#include <stddef.h>
#include <stdint.h>

volatile size_t sink;   // 防止优化

int vsetvl_vec_test() {
    size_t vl;

    vl = __riscv_vsetvl_e8m1(1024);
    sink = vl;

    // vl = __riscv_vsetvl_e16m2(256);
    // sink = vl;

    // vl = __riscv_vsetvl_e32m4(64);
    // sink = vl;

    // vl = __riscv_vsetvlmax_e8m1();
    // sink = vl;

    // vl = __riscv_vsetvlmax_e64m2();
    // sink = vl;

    return 0;
}