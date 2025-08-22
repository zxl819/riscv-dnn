// 选择实现：默认使用 RVV 版本；仅当显式启用 ENABLE_RVM_MATRIX 时使用 RVM 版本。
#ifdef ENABLE_RVM_MATRIX
#include "matmul_rvm.h"
#else
#include "matmul_rvv.h"
#endif

