# conda activate py3.9
# python3 test.py

#!/usr/bin/python3
import os
import sys
import numpy as np
import pandas as pd
import math

sys.path.append("D:/riscv-dnn")
from utils.check import from_txt, check_to_txt   # 从文本读取数据并比较
from utils.work import do_test   # 执行测试

title = " test for matmul operator"

# opt_levels = {"rvv_fp16acc":"-O2 -DFP16_ACC16", "rvv":"-O2"}
# opt_levels = {"rvv":"-O2", "rvm":"-O2 -D__RVM__" }
opt_levels = {"rvm":"-O2 -D__RVM__" }

# 输出结果列表的列名称
cols = ['Workload', 'Cycles', 'IPC', 'Front', 'BS', 'MEM', 'CORE', 'Retire']


simulator = 'spike'

print("run on %s" % simulator)

# 执行矩阵乘法并生成随机数作为输入数据，保存到文件中
# num测试编号 ； mnk矩阵维度
def matmul(num, m, k, n):
    vs1 = np.random.random((m, k)).astype('float16') * 2 - 1  # m*k矩阵，值的范围[-1,1]
    vs2 = np.random.random((k, n)).astype('float16') * 2 - 1  # k*n矩阵，值的范围[-1,1]
    vd = np.matmul(vs1, vs2, dtype=np.float16)  # 进行矩阵乘法，结果保存在vd

    vs1.tofile(f"./build/{num}/src1.bin")
    vs2.tofile(f"./build/{num}/src2.bin")
    vs1.tofile(f"src1.bin")
    vs2.tofile(f"src2.bin")
    vd.tofile(f"build/{num}golden.bin")

    return vd     # 返回标准结果

# 进行一次完整的测试，生成矩阵数据、编译并运行程序、验证输出结果
# num测试编号 ；params矩阵维度 ; defs编译时候的宏定义 ; ncores使用的核心数

# def test(num, params, defs, ncores=8):
def test(num, params, defs, ncores=1):
    m, k, n = params    # 读取矩阵维度

    os.system(f"rm -rf build/{num} && mkdir -p build/{num}")  # 重新构建测试目录
    
    golden = matmul(num, m, k, n)   # golden result
    out_size = hex(math.ceil((m * n  )/8)*8)  # 计算输出矩阵的大小

    # 执行make
    os.system(f"make DEFS='-DM={m} -DK={k} -DN={n} -DCORENUMS={ncores} {defs}' OUT_SIZE={out_size} run SIM={simulator} NUM={num} NCORES={ncores} >build/{num}/test.log 2>&1")

    # 从模拟器输出读出计算结果，并和gloden result对比
    result = from_txt( f'build/{num}/{simulator}.sig', golden, 0 )
    os.makedirs('check', exist_ok=True)   # 输出目录为check

    fp16acc = '-DFP16_ACC16' in defs
    # fp16acc use larger tolerances
    if fp16acc:
        rk = k * 1000
        ak = k * 10000
    else:
        rk = k
        ak = k
    check_result = check_to_txt( golden, result, f'check/{num}.data',
                     f'np.allclose( result, golden, rtol={1e-5*rk}, atol={1e-8*ak}, equal_nan=True)' )
    print(f"> {m}x{k}x{n}, check result: {check_result}")


if __name__ == "__main__":
    # perf params
    params = (
        #  m k n
        #(16, 16, 16),
        # (32, 32, 32),
        #  (64, 64, 64),
        # (32,32,32) ,
       # (16384,300,300),
       (100,100,100),
    )
    
    do_test(params, opt_levels, test, title, simulator, False)
  

