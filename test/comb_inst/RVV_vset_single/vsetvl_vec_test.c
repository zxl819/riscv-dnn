#include <riscv_vector.h>
#include <stddef.h>

int vsetvl_vec_test() {
    int vl;
    
    vl = vsetvl_e8m1(1024);
    vl = vsetvl_e8m2(1024);
    vl = vsetvl_e8m4(1024);
    vl = vsetvl_e8m8(1024);

    vl = vsetvl_e16m1(1024);
    vl = vsetvl_e16m2(1024);
    vl = vsetvl_e16m4(1024);
    vl = vsetvl_e16m8(1024);

    vl = vsetvl_e32m1(1024);
    vl = vsetvl_e32m2(1024);
    vl = vsetvl_e32m4(1024);
    vl = vsetvl_e32m8(1024);

    vl = vsetvl_e64m1(1024);
    vl = vsetvl_e64m2(1024);
    vl = vsetvl_e64m4(1024);
    vl = vsetvl_e64m8(1024);

    vl = vsetvl_e8m1(8);
    vl = vsetvl_e8m2(8);
    vl = vsetvl_e8m4(8);
    vl = vsetvl_e8m8(8);

    vl = vsetvl_e16m1(8);
    vl = vsetvl_e16m2(8);
    vl = vsetvl_e16m4(8);
    vl = vsetvl_e16m8(8);

    vl = vsetvl_e32m1(8);
    vl = vsetvl_e32m2(8);
    vl = vsetvl_e32m4(8);
    vl = vsetvl_e32m8(8);

    vl = vsetvl_e64m1(8);
    vl = vsetvl_e64m2(8);
    vl = vsetvl_e64m4(8);
    vl = vsetvl_e64m8(8);

    vl = vsetvlmax_e8m1(4);
    vl = vsetvlmax_e8m8(4);
    vl = vsetvlmax_e8m1(2048);
    vl = vsetvlmax_e8m8(2048);

    vl = vsetvlmax_e16m1(4);
    vl = vsetvlmax_e16m8(4);
    vl = vsetvlmax_e16m1(2048);
    vl = vsetvlmax_e16m8(2048);

    return vl;
}
