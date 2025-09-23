#!/usr/bin/env python3
import os
import math
import argparse
import numpy as np

def write_data_h(path, m, k, n, A_i8, B_i8, D_i32):
    def fmt_i8(arr, per_line=32):
        v = arr.astype(np.int8).reshape(-1)
        lines = []
        for i in range(0, v.size, per_line):
            chunk = ", ".join(str(int(x)) for x in v[i:i+per_line])
            lines.append("  " + chunk)
        return ",\n".join(lines)

    def fmt_i32(arr, per_line=16):
        v = arr.astype(np.int32).reshape(-1)
        lines = []
        for i in range(0, v.size, per_line):
            chunk = ", ".join(str(int(x)) for x in v[i:i+per_line])
            lines.append("  " + chunk)
        return ",\n".join(lines)

    content = f"""\
#pragma once
#include <stdint.h>

#define M {m}
#define K {k}
#define N {n}

/* 行主序(row-major)展平存储 */
__attribute__((aligned(64))) static const int8_t A_i8[M*K] = {{
{fmt_i8(A_i8)}
}};

__attribute__((aligned(64))) static const int8_t B_i8[K*N] = {{
{fmt_i8(B_i8)}
}};

/* C 作为输出缓冲，初始为0 */
__attribute__((aligned(64))) static int32_t C_i32[M*N] = {{
{", ".join(["1"] * (m*n))}
}};

/* D 为 B转置后的运算结果 golden 结果（int32 累加） */
__attribute__((aligned(64))) static const int32_t D_i32[M*N] = {{
{fmt_i32(D_i32)}
}};
"""
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", newline="\n") as f:
        f.write(content)

def gen_inputs_i8(m, k, n, seed=1, low=-128, high=127):
    rng = np.random.default_rng(seed)
    A = rng.integers(low, high + 1, size=(m, k), dtype=np.int16).astype(np.int8)
    B = rng.integers(low, high + 1, size=(k, n), dtype=np.int16).astype(np.int8)
    return A, B

def matmul_i8_i32(A_i8, B_i8):
    # int8 x int8 -> int32 累加
    # return (A_i8.astype(np.int32) @ B_i8.astype(np.int32).T).astype(np.int32)
    return (A_i8.astype(np.int32) @ B_i8.astype(np.int32)).astype(np.int32)

def matmul_and_dump(num, m, k, n, seed=1):
    outdir = os.path.join("build", str(num))
    os.makedirs(outdir, exist_ok=True)

    A_i8, B_i8 = gen_inputs_i8(m, k, n, seed=seed)
    D_i32 = matmul_i8_i32(A_i8, B_i8)

    # 保存二进制（行主序）
    A_i8.tofile(os.path.join(outdir, "src1.bin"))     # int8
    B_i8.tofile(os.path.join(outdir, "src2.bin"))     # int8
    D_i32.tofile(os.path.join(outdir, "golden.bin"))  # int32 (LE)

    return A_i8, B_i8, D_i32

def write_header(num, m, k, n, seed=1):
    A_i8, B_i8, D_i32 = matmul_and_dump(num, m, k, n, seed=seed)
    header_path = os.path.join("./build", str(num), "data.h")
    write_data_h(header_path, m, k, n, A_i8, B_i8, D_i32)
    print(f"[OK] 生成 {header_path}")
    print(f"     已生成二进制：./build/{num}/src1.bin (int8), src2.bin (int8), golden.bin (int32)")

def main():
    parser = argparse.ArgumentParser(description="Generate data.h (A,B int8; C=0,D int32) for matmul C=A*B")
    parser.add_argument("--m", type=int, required=True)
    parser.add_argument("--k", type=int, required=True)
    parser.add_argument("--n", type=int, required=True)
    parser.add_argument("--num", type=int, default=0, help="output under build/{num}")
    parser.add_argument("--seed", type=int, default=1)
    args = parser.parse_args()
    write_header(args.num, args.m, args.k, args.n, seed=args.seed)

if __name__ == "__main__":
    main()