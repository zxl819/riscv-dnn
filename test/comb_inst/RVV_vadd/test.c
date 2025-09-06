#include <riscv_vector.h>

vint32m1_t foo(vint32m1_t a, vint32m1_t b, size_t vl) {
    return vadd_vv_i32m1(a, b, vl);
}