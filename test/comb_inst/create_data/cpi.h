

#include <stdint.h>

int cid = 0;  // 或者根据你的硬件/软件平台定义合适的值

#define stringify_1(s) #s
#define stringify(s) stringify_1(s)
#define stats(code, iter) do { \
    unsigned long _c = -read_csr(mcycle), _i = -read_csr(minstret); \
    code; \
    _c += read_csr(mcycle), _i += read_csr(minstret); \
    if (cid == 0) \
      printf("\n%s: %ld cycles,%ld instructions, %ld.%ld cycles/iter, %ld.%ld CPI\n", \
             stringify(code), _c,_i, _c/iter, 10*_c/iter%10, _c/_i, 10*_c/_i%10); \
  } while(0)
