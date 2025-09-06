#ifndef __INCBIN_H__
#define __INCBIN_H__

#define INCBIN(var, binfile, section) \
    extern uint8_t var[]; \
    __asm__(".section " section ", \"a\", @progbits\n\t" \
            ".global " #var "\n\t" \
            ".type " #var ", @object \n\t" \
            ".align 20 \n" \
            #var ":\n\t" \
            ".incbin  \"" binfile "\"\n\t")

        
#endif // __INCBIN_H__

#ifdef DISABLE_INCBIN
#define INCBIN(name, file) \
  __attribute__((aligned(16))) const unsigned char name[] = {0}; \
  const unsigned int name##_len = 0;
#else
// 原有 INCBIN 宏定义
#endif
