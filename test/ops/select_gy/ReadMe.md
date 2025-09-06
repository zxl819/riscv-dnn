## 算子介绍

该文件实现一个基于 RISC‑V Matrix 扩展的查表+全矩阵广播函数 select_rvm，用于把一张 8x8 FP16 输出矩阵全部填成查表得到的标量值

## 编译生成可执行文件

```bash
clang --target=riscv64-unknown-elf -march=rv64gv0p10zfh0p1 -O2 -menable-experimental-extensions -mllvm -riscv-v-vector-bits-min=128 -o test.elf test.c
```

## 生成汇编代码

```bash
## 汇编编译
clang --target=riscv64-unknown-elf -march=rv64gv0p10zfh0p1 -O2 -menable-experimental-extensions -mllvm -riscv-v-vector-bits-min=128 -S -o test.S test.c
```

