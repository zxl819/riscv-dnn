// ...existing code...
#include <riscv_vector.h>
#include <stddef.h>

volatile size_t sink;

int main() {
    size_t vl;

    //AVL = 1024
    vl = __riscv_vsetvl_e8m1(1024);   sink = vl;
    vl = __riscv_vsetvl_e8m2(1024);   sink = vl;
    vl = __riscv_vsetvl_e8m4(1024);   sink = vl;
    vl = __riscv_vsetvl_e8m8(1024);   sink = vl;

    vl = __riscv_vsetvl_e16m1(1024);  sink = vl;
    vl = __riscv_vsetvl_e16m2(1024);  sink = vl;
    vl = __riscv_vsetvl_e16m4(1024);  sink = vl;
    vl = __riscv_vsetvl_e16m8(1024);  sink = vl;

    vl = __riscv_vsetvl_e32m1(1024);  sink = vl;
    vl = __riscv_vsetvl_e32m2(1024);  sink = vl;
    vl = __riscv_vsetvl_e32m4(1024);  sink = vl;
    vl = __riscv_vsetvl_e32m8(1024);  sink = vl;

    vl = __riscv_vsetvl_e64m1(1024);  sink = vl;
    vl = __riscv_vsetvl_e64m2(1024);  sink = vl;
    vl = __riscv_vsetvl_e64m4(1024);  sink = vl;
    vl = __riscv_vsetvl_e64m8(1024);  sink = vl;

    // AVL = 8
     vl = __riscv_vsetvl_e8m1(8);      sink = vl;
    vl = __riscv_vsetvl_e8m2(8);      sink = vl;
    vl = __riscv_vsetvl_e8m4(8);      sink = vl;
    vl = __riscv_vsetvl_e8m8(8);      sink = vl;

    vl = __riscv_vsetvl_e16m1(8);     sink = vl;
    vl = __riscv_vsetvl_e16m2(8);     sink = vl;
    vl = __riscv_vsetvl_e16m4(8);     sink = vl;
    vl = __riscv_vsetvl_e16m8(8);     sink = vl;

    vl = __riscv_vsetvl_e32m1(8);     sink = vl;
    vl = __riscv_vsetvl_e32m2(8);     sink = vl;
    vl = __riscv_vsetvl_e32m4(8);     sink = vl;
    vl = __riscv_vsetvl_e32m8(8);     sink = vl;

    vl = __riscv_vsetvl_e64m1(8);     sink = vl;
    vl = __riscv_vsetvl_e64m2(8);     sink = vl;
    vl = __riscv_vsetvl_e64m4(8);     sink = vl;
    vl = __riscv_vsetvl_e64m8(8);     sink = vl;

    // max VL（无参数）
    // vl = __riscv_vsetvlmax_e8m1();    sink = vl;
    // vl = __riscv_vsetvlmax_e8m8();    sink = vl;
    // vl = __riscv_vsetvlmax_e16m1();   sink = vl;
    // vl = __riscv_vsetvlmax_e16m8();   sink = vl;
    // vl = __riscv_vsetvlmax_e32m1();   sink = vl;
    // vl = __riscv_vsetvlmax_e32m8();   sink = vl;
    // vl = __riscv_vsetvlmax_e64m1();   sink = vl;
    // vl = __riscv_vsetvlmax_e64m8();   sink = vl;

    // return (int)vl;
    return 0;
}
// ...existing code...