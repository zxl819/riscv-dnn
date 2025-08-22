import os
import sys
import numpy as np
import pandas as pd
import math


sys.path.append("/nfs/home/zhengsihan/Code/new-tests/src-tests/riscv-dnn")
from utils.check import from_txt, check_to_txt   # 从文本读取数据并比较
from utils.work import do_test   # 执行测试



title = " test for mla operator"

opt_levels = {"rvm":"-O2 -D__RVM__" }


simulator = 'spike'

print("run on %s" % simulator)



def mla (num,nd ,nL ,ndcq ,nnh  ,ndh  ,ndhr ,ndckv):
    vht = np.random.random((nd,nL)).astype('float16') * 2 - 1
    vwdq = np.random.random((ndcq,nd)).astype('float16') * 2 - 1
    vwuq = np.random.random((nnh*ndh, ndcq)).astype('float16') * 2 - 1
    vwqr = np.random.random((nnh*ndh, ndcq)).astype('float16') * 2 - 1
    vwdkv = np.random.random((ndckv, nL)).astype('float16') * 2 - 1
    vwuk = np.random.random((nnh*ndh,ndckv )).astype('float16') * 2 - 1
    vwuv = np.random.random((nnh*ndh,ndckv )).astype('float16') * 2 - 1
    vwkr = np.random.random((ndhr,nd )).astype('float16') * 2 - 1
    vwo = np.random.random((nd,nnh*ndh )).astype('float16') * 2 - 1

    vd = np.matmul(vwdq, vht, dtype=np.float16)

    vht.tofile(f"build/{num}/ht.bin")
    vwdq.tofile(f"build/{num}/wdq.bin")
    vwuq.tofile(f"build/{num}/wuq.bin")
    vwqr.tofile(f"build/{num}/wqr.bin")
    vwdkv.tofile(f"build/{num}/wdkv.bin")
    vwuk.tofile(f"build/{num}/wuk.bin")
    vwuv.tofile(f"build/{num}/wuv.bin")
    vwkr.tofile(f"build/{num}/wkr.bin")
    vwo.tofile(f"build/{num}/wo.bin")

    vht.tofile(f"ht.bin")
    vwdq.tofile(f"wdq.bin")
    vwuq.tofile(f"wuq.bin")
    vwqr.tofile(f"wqr.bin")
    vwdkv.tofile(f"wdkv.bin")
    vwuk.tofile(f"wuk.bin")
    vwuv.tofile(f"wuv.bin")
    vwkr.tofile(f"wkr.bin")
    vwo.tofile(f"wo.bin")

    return vd



def test(num,params,defs,ncores=1):
    nd ,nL ,ndcq ,nnh  ,ndh  ,ndhr ,ndckv =params
    os.system(f"rm -rf build/{num} && mkdir -p build/{num}")  # 重新构建测试目录

    # golden?
    golden = mla(num,nd ,nL ,ndcq ,nnh  ,ndh  ,ndhr ,ndckv)
    # golden = matmul(num, m, k, n)   # golden result
    out_size = hex(math.ceil((nd * nL  )/8)*8)  # 计算输出矩阵的大小

    
    # 执行make
    os.system(f"make DEFS='-DCORENUMS={ncores} {defs}' OUT_SIZE={out_size} run SIM={simulator} NUM={num} NCORES={ncores} >build/{num}/test.log 2>&1")
    result = from_txt( f'build/{num}/{simulator}.sig', golden, 0 )
    os.makedirs('check', exist_ok=True)   # 输出目录为check

    

if __name__ == "__main__":
    # perf params
    params = (
        #  nd ,nL ,ndcq ,nnh  ,ndh  ,ndhr ,ndckv 
        (128, 4096, 1536,128,128,64,512) ,
        
    )
    
    # do_test(params, opt_levels, test, title, simulator, simulator!='spike')
    do_test(params, opt_levels, test, title, simulator, False)

