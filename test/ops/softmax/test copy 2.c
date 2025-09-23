#define VLEN 1024
#include <riscv_vector.h>
#define INFINITY (__builtin_inff())
#include <stdio.h>
#include <string.h>

// 简单十六进制/十进制转字符串与打印（不依赖 printf 的格式化）
static inline char dbg_hex_digit(unsigned x){ return (x<10)?('0'+x):('A'+(x-10)); }

static void dbg_print_line(const char* s) {
    // printf 在你环境中只打印第一个参数，直接传入一整行字符串即可
    printf(s);
}

static void dbg_print_u32(const char* label, uint32_t v) {
    char buf[64];
    int p = 0;
    // 复制 label
    for (; label[p] && p < (int)sizeof(buf)-1; ++p) buf[p] = label[p];
    if (p < (int)sizeof(buf)-1) buf[p++] = ':'; 
    if (p < (int)sizeof(buf)-1) buf[p++] = ' ';
    // 十进制
    char tmp[16]; int t = 0;
    if (v == 0) tmp[t++] = '0';
    else { while (v && t < (int)sizeof(tmp)) { tmp[t++] = '0' + (v % 10); v /= 10; } }
    while (t && p < (int)sizeof(buf)-1) buf[p++] = tmp[--t];
    if (p < (int)sizeof(buf)-1) buf[p++] = '\n';
    buf[p] = 0;
    printf(buf);
}

static void dbg_print_hex32(const char* label, uint32_t v) {
    char buf[64];
    int p = 0;
    for (; label[p] && p < (int)sizeof(buf)-1; ++p) buf[p] = label[p];
    if (p < (int)sizeof(buf)-1) buf[p++] = ':'; 
    if (p < (int)sizeof(buf)-1) buf[p++] = ' ';
    if (p < (int)sizeof(buf)-1) buf[p++] = '0';
    if (p < (int)sizeof(buf)-1) buf[p++] = 'x';
    for (int i = 7; i >= 0 && p < (int)sizeof(buf)-1; --i) {
        buf[p++] = dbg_hex_digit((v >> (i*4)) & 0xF);
    }
    if (p < (int)sizeof(buf)-1) buf[p++] = '\n';
    buf[p] = 0;
    printf(buf);
}

static void dbg_print_idx_hex32(const char* name, uint32_t idx, const char* suffix, uint32_t v) {
    char buf[96];
    int p = 0;
    // name
    for (; name[p] && p < (int)sizeof(buf)-1; ++p) buf[p] = name[p];
    if (p < (int)sizeof(buf)-1) buf[p++] = '[';
    // idx
    char tmp[16]; int t = 0; uint32_t x = idx;
    if (x == 0) tmp[t++] = '0';
    else { while (x && t < (int)sizeof(tmp)) { tmp[t++] = '0' + (x % 10); x /= 10; } }
    while (t && p < (int)sizeof(buf)-1) buf[p++] = tmp[--t];
    if (p < (int)sizeof(buf)-1) buf[p++] = ']';
    if (suffix) {
        if (p < (int)sizeof(buf)-1) buf[p++] = ' ';
        for (int i = 0; suffix[i] && p < (int)sizeof(buf)-1; ++i) buf[p++] = suffix[i];
    }
    if (p < (int)sizeof(buf)-1) buf[p++] = ':';
    if (p < (int)sizeof(buf)-1) buf[p++] = ' ';
    if (p < (int)sizeof(buf)-1) buf[p++] = '0';
    if (p < (int)sizeof(buf)-1) buf[p++] = 'x';
    for (int i = 7; i >= 0 && p < (int)sizeof(buf)-1; --i) {
        buf[p++] = dbg_hex_digit((v >> (i*4)) & 0xF);
    }
    if (p < (int)sizeof(buf)-1) buf[p++] = '\n';
    buf[p] = 0;
    printf(buf);
}

static inline uint32_t load_f32_bits(const float* p) {
    uint32_t u; memcpy(&u, p, 4); return u;
}

void softmax_stable_rvv_fp32(float* dst, float* src, size_t n);
float quick_dirty_vector_expf(float* dst, float* src, float max_x, size_t n);
uint32_t quick_dirty_vector_expf_no_scalar(float* dst, float* src, uint32_t max_x_bits, size_t n);
// void *memset(void *s, int c, size_t n);
// void halt(int code);
// void _trm_init();
// int main();
// void _exit(int code) ;
float src[VLEN/32]={-0.50183952f,1.80285728f,0.92797577f,0.39463395f,-1.37592542f,-1.37602186f,-1.76766551f,1.46470463f,0.40446004f,0.83229029f,-1.91766202f,1.87963939f,1.32977057f,-1.15064359f,-1.27270019f,-1.26638198f,-0.78303105f,0.09902573f,-0.27221993f,-0.83508343f,0.44741157f,-1.44202459f,-0.83142143f,-0.53455263f,-0.17572007f,1.14070380f,-1.20130491f,0.05693775f,0.36965826f,-1.81419837f,0.43017942f,-1.31790352f};
float dst[VLEN/32]={0};

float golden[VLEN/32]={0.01340518f,0.13433519f,0.05600587f,0.03285535f,0.00559322f,0.00559268f,0.00378034f,0.09579259f,0.03317978f,0.05089533f,0.00325378f,0.14505604f,0.08370104f,0.00700649f,0.00620143f,0.00624074f,0.01011935f,0.02444698f,0.01686534f,0.00960609f,0.03463596f,0.00523547f,0.00964133f,0.01297375f,0.01857396f,0.06928197f,0.00666038f,0.02343940f,0.03204493f,0.00360847f,0.03404422f,0.00592735f};
float diff_mem[VLEN/32]={0};
// volatile uint64_t tohost __attribute__((aligned(8)));
// volatile uint64_t fromhost __attribute__((aligned(8)));
int main(){
    softmax_stable_rvv_fp32(dst,src,VLEN/32);
    // size_t vl = __riscv_vsetvlmax_e32m1();
    // vfloat32m1_t vx = __riscv_vle32_v_f32m1(gloden, vl);
    // vfloat32m1_t vy = __riscv_vle32_v_f32m1(dst, vl);
    // vfloat32m1_t diff = __riscv_vfsub_vv_f32m1(vx, vy, vl);

    // __riscv_vse32(diff_mem, diff, vl);
    return 0;

}


// 完全避免标量浮点寄存器的版本
void softmax_stable_rvv_fp32(float* dst, float* src, size_t n)
{
    const size_t vlmax = __riscv_vsetvlmax_e32m1(); 
    
    // 使用内存数组存储所有常量，通过整数指针访问
    static const uint32_t constants[] = {
        0xFF800000,  // -INFINITY [0]
        0x3F317218,  // ln(2)     [1]
        0x3FB8AA3B,  // 1/ln(2)   [2]  
        0x3E800000,  // 0.25f     [3]
        0x40000000,  // 2.0f      [4]
        0x3F800000,  // 1.0f      [5]
        0x3EFFFFFC,  // poly_c_2  [6]
        0x3D2AA427,  // poly_c_3  [7]
        0x3AAAB664,  // poly_c_4  [8]
        0x38891F99,  // poly_c_5  [9]
        0x3684250E,  // poly_c_6  [10]
        0x322BCC77   // 1e-8f     [11]
    };
    
    printf("vlmax = %zu\n", vlmax);
    
    // // 通过整数向量加载，然后重新解释为浮点向量
    // vuint32m1_t vconst_int = __riscv_vle32_v_u32m1(constants, vlmax);
    // vfloat32m1_t vconst = __riscv_vreinterpret_v_u32m1_f32m1(vconst_int);
    
    // // 提取 -INFINITY 常量（索引0）
    // vfloat32m1_t vmax = __riscv_vslidedown_vx_f32m1_tu(__riscv_vundefined_f32m1(), vconst, 0, vlmax);
   vuint32m1_t vneg_inf_int = __riscv_vmv_v_x_u32m1(constants[0], vlmax);
   vfloat32m1_t vmax = __riscv_vreinterpret_v_u32m1_f32m1(vneg_inf_int);
    // 打印输入数据
    dbg_print_line("Input data (first 8, bits):\n");
    for (size_t i = 0; i < n && i < 8; i++) {
        dbg_print_idx_hex32("src", (uint32_t)i, "bits", load_f32_bits(&src[i]));
    }

    size_t avl = n;
    while (avl > 0) {
        size_t vl = __riscv_vsetvl_e32m1(avl);
        vfloat32m1_t vx = __riscv_vle32_v_f32m1(src, vl);
        //vmax = __riscv_vfmax_tu(vmax, vx, vmax, vl);
        vmax = __riscv_vfmax_vv_f32m1(vmax, vx, vl);
        avl -= vl;
        src += vl;
    }
    src -= n; // reseting source pointer

    // final maximum reduction - 重新加载 -INFINITY
    //vuint32m1_t vneg_inf_int = __riscv_vmv_v_x_u32m1(constants[0], vlmax);
    vfloat32m1_t vredmax = __riscv_vreinterpret_v_u32m1_f32m1(vneg_inf_int);
    vredmax = __riscv_vfredmax_vs_f32m1_f32m1(vmax, vredmax, vlmax);
    
    // 通过内存获取 max_x，避免标量浮点寄存器
    uint32_t max_x_bits[1];
    vuint32m1_t vredmax_int = __riscv_vreinterpret_v_f32m1_u32m1(vredmax);
    __riscv_vse32_v_u32m1(max_x_bits, vredmax_int, 1);
    // 通过联合体避免直接浮点访问
    union { uint32_t u; float f; } max_x_union = {.u = max_x_bits[0]};
    uint32_t max_x_as_int = max_x_union.u;
    printf("sum bits: 0x%08X\n", max_x_as_int);
    printf("max_x = %f (bits: 0x%08X)\n", max_x_union.f, max_x_as_int);


    // Computing element-wise exponentials and their sum.
    uint32_t sum_bits = quick_dirty_vector_expf_no_scalar(dst, src, max_x_as_int, n);
    dbg_print_hex32("sum bits", sum_bits);
    // 打印 sum
    union { uint32_t u; float f; } sum_union = {.u = sum_bits};
    //printf("sum = %f (bits: 0x%08X)\n", sum_union.f, sum_bits);

    // 完全向量化的 Newton-Raphson 倒数计算
    vuint32m1_t vsum_int = __riscv_vmv_v_x_u32m1(sum_bits, vlmax);
    vfloat32m1_t vsum = __riscv_vreinterpret_v_u32m1_f32m1(vsum_int);
    
    // 提取常量 epsilon (索引11), init_val (索引3), two_val (索引4)
    vuint32m1_t vepsilon_int = __riscv_vmv_v_x_u32m1(constants[11], vlmax);
    vfloat32m1_t vepsilon = __riscv_vreinterpret_v_u32m1_f32m1(vepsilon_int);
    vfloat32m1_t vsum_safe = __riscv_vfadd_vv_f32m1(vsum, vepsilon, vlmax);
    
    // Newton-Raphson 初值 (0.25f)
    vuint32m1_t vx0_int = __riscv_vmv_v_x_u32m1(constants[3], vlmax);
    vfloat32m1_t vx0 = __riscv_vreinterpret_v_u32m1_f32m1(vx0_int);
    
    // Newton-Raphson 第1次迭代: x1 = x0 * (2 - sum * x0)
    vuint32m1_t vtwo_int = __riscv_vmv_v_x_u32m1(constants[4], vlmax);
    vfloat32m1_t vtwo = __riscv_vreinterpret_v_u32m1_f32m1(vtwo_int);
    vfloat32m1_t vtemp = __riscv_vfmul_vv_f32m1(vsum, vx0, vlmax);     // sum * x0
    vtemp = __riscv_vfsub_vv_f32m1(vtwo, vtemp, vlmax);                // 2 - sum * x0
    vfloat32m1_t vx1 = __riscv_vfmul_vv_f32m1(vx0, vtemp, vlmax);     // x1 = x0 * (2 - sum * x0)
    
    // Newton-Raphson 第2次迭代: x2 = x1 * (2 - sum * x1) 提高精度
    vfloat32m1_t vtemp2 = __riscv_vfmul_vv_f32m1(vsum, vx1, vlmax);   // sum * x1
    vtemp2 = __riscv_vfsub_vv_f32m1(vtwo, vtemp2, vlmax);             // 2 - sum * x1
    vfloat32m1_t vinv_sum = __riscv_vfmul_vv_f32m1(vx1, vtemp2, vlmax); // x2 = x1 * (2 - sum * x1)

    // 打印倒数结果
    uint32_t inv_sum_bits[1];
    vuint32m1_t vinv_sum_debug = __riscv_vreinterpret_v_f32m1_u32m1(vinv_sum);
    __riscv_vse32_v_u32m1(inv_sum_bits, vinv_sum_debug, 1);
    union { uint32_t u; float f; } inv_sum_union = {.u = inv_sum_bits[0]};
    dbg_print_hex32("inv_sum bits", inv_sum_bits[0]);

    // normalizing each element using vector-vector multiplication
    avl = n;
    dst -= n; // reset dst pointer
    while (avl > 0) {
        size_t vl = __riscv_vsetvl_e32m1(avl);
        vfloat32m1_t row = __riscv_vle32_v_f32m1(dst, vl);
        
        // 提取标量值通过整数操作
        uint32_t scalar_inv_bits[1];
        vuint32m1_t vinv_sum_int = __riscv_vreinterpret_v_f32m1_u32m1(vinv_sum);
        __riscv_vse32_v_u32m1(scalar_inv_bits, vinv_sum_int, 1);
        vuint32m1_t vinv_slice_int = __riscv_vmv_v_x_u32m1(scalar_inv_bits[0], vl);
        vfloat32m1_t vinv_slice = __riscv_vreinterpret_v_u32m1_f32m1(vinv_slice_int);
        
        row = __riscv_vfmul_vv_f32m1(row, vinv_slice, vl);
        volatile vfloat32m1_t vx = __riscv_vle32_v_f32m1(golden, vl);
        __riscv_vse32_v_f32m1(dst, row, vl);
        avl -= vl;
        dst += vl;
    }
    
    // 打印最终结果
    dbg_print_line("Final results (first 8, bits):\n");
    for (size_t i = 0; i < n && i < 8; i++) {
        dbg_print_idx_hex32("dst", (uint32_t)i, "bits", load_f32_bits(&dst[i]));
        dbg_print_idx_hex32("golden", (uint32_t)i, "bits", load_f32_bits(&golden[i]));
    }
}

/** RVV-based vectorized implementation avoiding scalar FP registers */
uint32_t quick_dirty_vector_expf_no_scalar(float* dst, float* src, uint32_t max_x_bits, size_t n) {
    // 常量数组，通过整数访问
    static const uint32_t exp_constants[] = {
    0x3F317218,  // ln(2)
    0x3FB8AA3B,  // 1/ln(2)
    0x00000000,  // 0.0f
    0x3F800000,  // c0 = 1.0
    0x3F800000,  // c1 = 1.0
    0x3F000000,  // c2 = 1/2 = 0.5
    0x3E2AAAAB,  // c3 = 1/6 ≈ 0.16666667
    0x3D2AAAAB,  // c4 = 1/24 ≈ 0.04166667
    0x3C088889,  // c5 = 1/120 ≈ 0.00833333
    0x3AB60B61   // c6 = 1/720 ≈ 0.00138889
    };
    dbg_print_hex32("exp max_x bits", max_x_bits);

    // union { uint32_t u; float f; } max_x_debug = {.u = max_x_bits};
    // printf("exp function: max_x = %f\n", max_x_debug.f);

    const size_t vlmax = __riscv_vsetvlmax_e32m1();
    
    // 通过整数向量创建浮点常量
    vuint32m1_t vln2_int = __riscv_vmv_v_x_u32m1(exp_constants[0], vlmax);
    vuint32m1_t viln2_int = __riscv_vmv_v_x_u32m1(exp_constants[1], vlmax);
    vuint32m1_t vzero_int = __riscv_vmv_v_x_u32m1(exp_constants[2], vlmax);
    
    vfloat32m1_t vln2 = __riscv_vreinterpret_v_u32m1_f32m1(vln2_int);
    vfloat32m1_t viln2 = __riscv_vreinterpret_v_u32m1_f32m1(viln2_int);
    //vfloat32m1_t vsum = __riscv_vreinterpret_v_u32m1_f32m1(vzero_int);
    __riscv_vsetvl_e32m1(1);
    vfloat32m1_t vsum = __riscv_vreinterpret_v_u32m1_f32m1(vzero_int);

    // 多项式系数
    vfloat32m1_t poly_c_0 = __riscv_vreinterpret_v_u32m1_f32m1(__riscv_vmv_v_x_u32m1(exp_constants[3], vlmax));
    vfloat32m1_t poly_c_1 = __riscv_vreinterpret_v_u32m1_f32m1(__riscv_vmv_v_x_u32m1(exp_constants[4], vlmax));
    vfloat32m1_t poly_c_2 = __riscv_vreinterpret_v_u32m1_f32m1(__riscv_vmv_v_x_u32m1(exp_constants[5], vlmax));
    vfloat32m1_t poly_c_3 = __riscv_vreinterpret_v_u32m1_f32m1(__riscv_vmv_v_x_u32m1(exp_constants[6], vlmax));
    vfloat32m1_t poly_c_4 = __riscv_vreinterpret_v_u32m1_f32m1(__riscv_vmv_v_x_u32m1(exp_constants[7], vlmax));
    vfloat32m1_t poly_c_5 = __riscv_vreinterpret_v_u32m1_f32m1(__riscv_vmv_v_x_u32m1(exp_constants[8], vlmax));
    vfloat32m1_t poly_c_6 = __riscv_vreinterpret_v_u32m1_f32m1(__riscv_vmv_v_x_u32m1(exp_constants[9], vlmax));

    // max_x 从整数转换为向量
    vuint32m1_t vmax_x_int = __riscv_vmv_v_x_u32m1(max_x_bits, vlmax);
    vfloat32m1_t vmax_x = __riscv_vreinterpret_v_u32m1_f32m1(vmax_x_int);

    asm volatile ("csrw frm, %0" : : "i" (0x0)); 
    size_t avl = n;
    while (avl > 0) {
        size_t vl = __riscv_vsetvl_e32m1(avl);
        vfloat32m1_t vx = __riscv_vle32_v_f32m1(src, vl);
        vx = __riscv_vfsub_vv_f32m1(vx, vmax_x, vl);  // 使用向量-向量减法

        // 打印减法后的第一个元素
        if (avl == n) {
            float debug_val[1];
            __riscv_vse32_v_f32m1(debug_val, vx, 1);
            printf("After subtract max: vx[0] = %f\n", debug_val[0]);
        }

        vfloat32m1_t vexp_vx;
        // argument reduction
        {
            unsigned old_frm;
            asm volatile("csrr %0, frm" : "=r"(old_frm));
            asm volatile("csrw frm, %0" :: "r"(0)); // RNE

            vfloat32m1_t vxiln2 = __riscv_vfmul_vv_f32m1(vx, viln2, vl);
            vint32m1_t   vk     = __riscv_vfcvt_x_f_v_i32m1(vxiln2, vl);

            asm volatile("csrw frm, %0" :: "r"(old_frm));

            // r = x - k*ln2
            vfloat32m1_t vfk  = __riscv_vfcvt_f_x_v_f32m1(vk, vl);
            vfloat32m1_t vkl2 = __riscv_vfmul_vv_f32m1(vfk, vln2, vl);
            vfloat32m1_t vr   = __riscv_vfsub_vv_f32m1(vx, vkl2, vl);

            // Horner: ((((((c6*r + c5)*r + c4)*r + c3)*r + c2)*r + c1)*r + c0)
            vfloat32m1_t p = poly_c_6;
            p = __riscv_vfmadd_vv_f32m1(p, vr, poly_c_5, vl);
            p = __riscv_vfmadd_vv_f32m1(p, vr, poly_c_4, vl);
            p = __riscv_vfmadd_vv_f32m1(p, vr, poly_c_3, vl);
            p = __riscv_vfmadd_vv_f32m1(p, vr, poly_c_2, vl);
            p = __riscv_vfmadd_vv_f32m1(p, vr, poly_c_1, vl);
            p = __riscv_vfmadd_vv_f32m1(p, vr, poly_c_0, vl);

            // 2^k 重建
            const int exp_bias = 127;
            vint32m1_t vbiased_exp = __riscv_vadd_vx_i32m1(vk, exp_bias, vl);
            vint32m1_t vexp2_vk    = __riscv_vsll_vx_i32m1(vbiased_exp, 23, vl);
            vfloat32m1_t vfexp2_vk = __riscv_vreinterpret_v_i32m1_f32m1(vexp2_vk);

            vexp_vx = __riscv_vfmul_vv_f32m1(p, vfexp2_vk, vl);
            // 后续继续用 vexp_vx
        }

        // 打印第一个exp值
        // if (avl == n) {
        //     float debug_exp[1];
        //     __riscv_vse32_v_f32m1(debug_exp, vexp_vx, 1);
        //     printf("First exp value: %f\n", debug_exp[0]);
        // }
         if (avl == n) {
            float debug_exp[1];
            __riscv_vse32_v_f32m1(debug_exp, vexp_vx, 1);
            printf("First exp value: %f\n", debug_exp[0]);
        }

        __riscv_vse32_v_f32m1(dst, vexp_vx, vl);

        // 块归约 -> 单lane，再累加到总和（vl=1），避免尾部污染
        vfloat32m1_t vblk = __riscv_vfredusum_vs_f32m1_f32m1(
                                vexp_vx,
                                __riscv_vreinterpret_v_u32m1_f32m1(vzero_int),
                                vl);
        __riscv_vsetvl_e32m1(1);
        vsum = __riscv_vfadd_vv_f32m1(vsum, vblk, 1);

        //__riscv_vse32_v_f32m1(dst, vexp_vx, vl);
        avl -= vl;
        src += vl;
        dst += vl;
    }

    // 归约并通过整数返回
    vfloat32m1_t vredsum = __riscv_vreinterpret_v_u32m1_f32m1(__riscv_vmv_v_x_u32m1(exp_constants[2], vlmax));
    vredsum = __riscv_vfredusum_vs_f32m1_f32m1(vsum, vredsum, vlmax);

    // 通过整数返回结果
    uint32_t result_bits[1];
    __riscv_vse32_v_u32m1(result_bits, __riscv_vreinterpret_v_f32m1_u32m1(vsum), 1);
     dbg_print_hex32("Returning sum bits", result_bits[0]);
    //  return result_bits[0];
    // vuint32m1_t vredsum_int = __riscv_vreinterpret_v_f32m1_u32m1(vredsum);
    // __riscv_vse32_v_u32m1(result_bits, vredsum_int, 1);
    
    // union { uint32_t u; float f; } result_debug = {.u = result_bits[0]};
    // dbg_print_hex32("Returning sum bits", result_bits[0]);
    
    return result_bits[0];
}





// ...existing code...
// void *memset(void *s, int c, size_t n) {
//     char *schar=s;
//     while(n--){
//       *schar++=c;
//     }
//     return s;
//   }

  
// void _trm_init() {
//     int ret = main();
//     _exit(0);
//     asm volatile("mv a0, %0; " : :"r"(ret));
//     while(1);
// }
  
// void _exit(int code) {
//     tohost = (code << 1) | 1;
//     while (1) ; // 防止跑飞
// }