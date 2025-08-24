#include <riscv_matrix.h>
#include <stddef.h>

size_t demo_settype_low(size_t v){
    return msettypei(v);
}
size_t demo_settype_hi(size_t v){
    if(v) return msettypehi(v);
    return 0;
}

int main(){
    size_t a = demo_settype_low(1);
    size_t b = demo_settype_hi(0x10);
    return (int)(a + b);
}
