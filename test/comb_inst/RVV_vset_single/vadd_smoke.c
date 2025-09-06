#include <riscv_vector.h>

vint8m1_t vadd_test(vint8m1_t a, vint8m1_t b, size_t vl) {
    return vadd_vv_i8m1(a, b, vl);
}