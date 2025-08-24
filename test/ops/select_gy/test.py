# conda activate py3.9
# python3 test.py

#!/usr/bin/python3
import os
import sys
import numpy as np
import pandas as pd
import math

#sys.path.append("/nfs/home/zhengsihan/Code/new-tests/src-tests/riscv-dnn")
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..')))
from utils.check import from_txt, check_to_txt   # 从文本读取数据并比较
from utils.work import do_test   # 执行测试

title = " test for select operator"

# opt_levels = {"rvv_fp16acc":"-O2 -DFP16_ACC16", "rvv":"-O2"}
# opt_levels = {"rvv":"-O2", "rvm":"-O2 -D__RVM__" }
opt_levels = {"rvm":"-O2 -D__RVM__" }

# 输出结果列表的列名称
cols = ['Workload', 'Cycles', 'IPC', 'Front', 'BS', 'MEM', 'CORE', 'Retire']


simulator = 'spike'

print("run on %s" % simulator)


# 预定义查表数据
der_1 = np.array([
    1.0, 1.0, 1.1641532182693481e-10, 
    2.3283064365386963e-10, 4.656612873077393e-10, 9.313225746154785e-10, 
    1.862645149230957e-09, 3.725290298461914e-09, 7.450580596923828e-09, 
    1.4901161193847656e-08, 2.9802322387695312e-08, 5.960464477539063e-08, 
    1.1920928955078125e-07, 2.384185791015625e-07, 4.76837158203125e-07, 
    9.5367431640625e-07, 1.9073486328125e-06, 3.814697265625e-06, 
    7.62939453125e-06, 1.52587890625e-05, 3.0517578125e-05, 
    6.103515625e-05, 0.0001220703125, 0.000244140625, 0.00048828125, 
    0.0009765625, 0.001953125, 0.00390625, 0.0078125, 0.015625, 
    0.03125, 0.0625, 0.125, 0.25, 0.5, 1.0, 2.0, 4.0, 8.0, 
    16.0, 32.0, 64.0, 128.0, 256.0, 512.0, 1024.0, 2048.0, 4096.0, 
    8192.0, 16384.0, 32768.0, 65536.0
], dtype=np.float16)

# 执行矩阵乘法并生成随机数作为输入数据，保存到文件中
# num测试编号 ； mnk矩阵维度
def select(num, index):
    value = der_1[index]  # 查表获取对应值
    results = np.full((8, 8), value, dtype=np.float16)  # 生成 8x8 矩阵，每个元素都是 der_1[index]

    # 保存输入索引和计算得到的标准结果
    indices = np.full((1,), index, dtype=np.int16)  # 存储单个索引值
    
    indices.tofile(f"build/{num}/indices.bin")  # 保存索引值
    indices.tofile(f"indices.bin")  
    results.tofile(f"build/{num}/golden.bin")  # 保存 8x8 结果矩阵
    results.tofile(f"golden.bin")  # 保存 8x8 结果矩阵
    return results

# def test(num, params, defs, ncores=8):
def test(num, params, defs, ncores=1):
    index = params    # 读取矩阵维度

    os.system(f"rm -rf build/{num} && mkdir -p build/{num}")  # 重新构建测试目录
    
    golden = select(num,index)   # golden result
    # 执行make
    os.system(f"make DEFS='-DINDEX={index} {defs}' run SIM={simulator} NUM={num}>build/{num}/test.log 2>&1")

    # 从模拟器输出读出计算结果，并和gloden result对比
    result = from_txt( f'build/{num}/{simulator}.sig', golden, 0 )
    os.makedirs('check', exist_ok=True)   # 输出目录为check

    check_result = check_to_txt( golden, result, f'check/{num}.data',
                     f'np.allclose( result, golden, rtol={1e-5}, atol={1e-8}, equal_nan=True)' )
    print(f"> {index}, check result: {check_result}")


if __name__ == "__main__":
    # perf params
    params = (
        (10),
    )
    
    do_test(params, opt_levels, test, title, simulator, False)
  

