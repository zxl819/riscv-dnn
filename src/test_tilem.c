#include <stdio.h>
#include "matmul_rvm_tile.h"

static void print_cb(int i, int j, int kk, int tm, int tn, int tk, void* user) {
    (void)user;
    printf("tile: i=%d, j=%d, k=%d | tm=%d, tn=%d, tk=%d\n", i, j, kk, tm, tn, tk);
}

int main() {
    int m = 100, n = 100, k = 100;
    tile_mnk_loop_cb(m, n, k, print_cb, NULL);
    return 0;
}
