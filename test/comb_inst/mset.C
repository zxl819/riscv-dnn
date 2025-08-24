#include <stdint.h>
#include <stdio.h>

void getMtype(uint64_t *csr_data, uint64_t *reg_data) {
    __asm__ volatile("csrr %0, 0xc40" : "=r"(*csr_data));
    __asm__ volatile("mv %0, x5" : "=r"(*reg_data));
}

void getMtilem(uint64_t *csr_data, uint64_t *reg_data) {
    __asm__ volatile("csrr %0, 0xc41" : "=r"(*csr_data));
    __asm__ volatile("mv %0, x4" : "=r"(*reg_data));
}

int main() {
    uint64_t csr_init, csr_msettypei, csr_msettype, csr_msettypehi, csr_msetsew, csr_msetint4;
    uint64_t x5_init, x5_msettypei, x5_msettype, x5_msettypehi, x5_msetsew, x5_msetint4;

    getMtype(&csr_init, &x5_init);
    __asm__ volatile("msettypei x5, e16");

    getMtype(&csr_msettypei, &x5_msettypei);

    uint32_t csr_temp = 0x12345678;
    __asm__ volatile("mv x6, %0" : : "r"(csr_temp) : );
    __asm__ volatile("msettype x5, x6");

    getMtype(&csr_msettype, &x5_msettype);

    __asm__ volatile("msettypehi x5, ba");
    getMtype(&csr_msettypehi, &x5_msettypehi);

    __asm__ volatile("msetsew x5, 1");
    getMtype(&csr_msetsew, &x5_msetsew);

    __asm__ volatile("msetint4 x5, 0");
    getMtype(&csr_msetint4, &x5_msetint4);

    uint64_t mtilem_init, x4_init;
    getMtilem(&mtilem_init, &x4_init);

    uint64_t mtiem_temp = 0xA;
    __asm__ volatile("mv x6, %0" : : "r"(mtiem_temp) : );
    __asm__ volatile("msettilem x4, x6");

    uint64_t mtilem_msettilem, x4_msettilem;
    getMtilem(&mtilem_msettilem, &x4_msettilem);

    printf("Init:\n");
    printf("  CSR[c40] = 0x%lx\n", csr_init);
    printf("  x5       = 0x%lx\n", x5_init);
    printf("After msettypei:\n");
    printf("  CSR[c40] = 0x%lx\n", csr_msettypei);
    printf("  x5       = 0x%lx\n", x5_msettypei);
    printf("After msettype:\n");
    printf("  CSR[c40] = 0x%lx\n", csr_msettype);
    printf("  x5       = 0x%lx\n", x5_msettype);
    printf("After msettypehi:\n");
    printf("  CSR[c40] = 0x%lx\n", csr_msettypehi);
    printf("  x5       = 0x%lx\n", x5_msettypehi);
    printf("After msetsew:\n");
    printf("  CSR[c40] = 0x%lx\n", csr_msetsew);
    printf("  x5       = 0x%lx\n", x5_msetsew);
    printf("After munsetint4:\n");
    printf("  CSR[c40] = 0x%lx\n", csr_msetint4);
    printf("  x5       = 0x%lx\n", x5_msetint4);
    printf("CSR[c41] (mtilem) = 0x%lx\n", mtilem_init);
    printf("  x5       = 0x%lx\n", x4_init);
    printf("CSR[c41] (mtilem) = 0x%lx\n", mtilem_msettilem);
    printf("  x4       = 0x%lx\n", x4_msettilem);

    return 0;
}