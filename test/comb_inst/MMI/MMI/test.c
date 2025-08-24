#include <riscv_matrix.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "../include/matrix_intrinsic.h"
#include "data.h"

#define DPRINT(x) \
    _Generic((x), \
        uint8_t:   printf("%02" PRIx8  " ",  (uint8_t)(x)), \
        uint16_t:  printf("%04" PRIx16 " ",  (uint16_t)(x)), \
        uint32_t:  printf("%08" PRIx32 " ",  (uint32_t)(x)), \
        uint64_t:  printf("%016" PRIx64 " ", (uint64_t)(x)), \
        default:   printf("Unknown type\n") \
    )


int main(){
    int value;
    asm volatile("csrr %0, %1" : "=r"(value) : "i"(0xc44));
    printf("mlenb(0xc44): %d, ",value);
    asm volatile("csrr %0, %1" : "=r"(value) : "i"(0xc45));
    printf("mrlenb(0xc45): %d \r\n",value);
    asm volatile("csrr %0, %1" : "=r"(value) : "i"(0xc40));
    printf("mtilem(0xc40): %d, ",value);
    asm volatile("csrr %0, %1" : "=r"(value) : "i"(0xc41));
    printf("mtilen(0xc41): %d \r\n",value);
    asm volatile("csrr %0, %1" : "=r"(value) : "i"(0xc42));
    printf("mtilek(0xc42): %d \r\n",value);

    int m, n, k;
    m = M; k = K; n = N;
    int tile_m = 0, tile_n = 0, tile_k = 0;  
    printf("m = %d, k = %d, n = %d\n",m,k,n);

    SET_DIM_TYPE;           // 以dest的msew为主
    tile_m = msettilem(m); 
    tile_n = msettilen(n);
    tile_k = msettilek(k);
    printf("tile_m %d, tile_n %d, tile_k %d\n", tile_m, tile_n, tile_k);
    asm volatile("csrr %0, %1" : "=r"(value) : "i"(0xc40));
    printf("mtilem(0xc40): %d, ",value);
    asm volatile("csrr %0, %1" : "=r"(value) : "i"(0xc41));
    printf("mtilen(0xc41): %d \r\n",value);
    asm volatile("csrr %0, %1" : "=r"(value) : "i"(0xc42));
    printf("mtilek(0xc42): %d \r\n",value);


    msrcDTYPE  tr0 = mla_m((srcDTYPE *)A, K*sizeof(srcDTYPE));
    msrcDTYPE  tr1 = mlb_m((srcDTYPE *)B, N*sizeof(srcDTYPE));
    mdestDTYPE acc = mlc_m((destDTYPE *)C, N*sizeof(destDTYPE));
    SET_CAL_TYPE;           // 以src的msew为主
    acc = MMA(acc, tr0, tr1);
    msc_m(acc, (destDTYPE *)O, N*sizeof(destDTYPE)); 

    printf("[ --- Golden Data --- ] \n");
    for(uint8_t i = 0; i < M; i = i + 1)  {
      for(uint8_t j = 0; j < N; j = j + 1) {
        DPRINT(D[i*N + j]);
      }
      printf("\n");
    }

    printf("[ --- Output Data --- ] \n");
    for(uint8_t i = 0; i < M; i = i + 1)  {
      for(uint8_t j = 0; j < N; j = j + 1) {
        DPRINT(O[i*N + j]); 
      }
      printf("\n");
    }

  #ifdef ENABLE_FILE_OUT
  FILE *fp = fopen("./tests/test-progs/MMI/outdata.bin", "wb");
  if (!fp) {
    perror("fopen");
    return 1;
  }
  fwrite(O, sizeof(destDTYPE), M*N, fp); 
  fclose(fp);
  #else
  // 简化环境：未启用标准文件 I/O，直接打印前 4 个输出元素作为示例
  printf("[ --- First 4 Output Values --- ]\n");
  for (int i=0;i<4 && i < M*N;i++) {
    DPRINT(O[i]);
  }
  printf("\n");
  #endif
    return 0;
}