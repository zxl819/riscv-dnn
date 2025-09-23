#!/usr/bin/python3
import os
import sys
import numpy as np
import pandas as pd

import tensorflow as tf


# sys.path.append("../../../utils") 
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..', '..')))
from utils.check import from_txt, check_to_txt
from utils.work import do_test


title = "test for softmax operator"
opt_levels = {"loop=1":"-O2"}

simulator = 'spike'
if len(sys.argv) > 1:
    simulator = sys.argv[1]
print("run on %s" % simulator)


def softmax(num, hin, win):
    # np.random.seed( 100 )
    #vs1 = np.random.random((hin, win)).astype('float32') * 10 - 5 # random  number between -5 and 5
    float vs1[16] = {-0.50183952f,1.80285728f,0.92797577f,0.39463395f,-1.37592542f,-1.37602186f,-1.76766551f,1.46470463f,0.40446004f,0.83229029f,-1.91766202f,1.87963939f,1.32977057f,-1.15064359f,-1.27270019f,-1.26638198f};
    vd = tf.nn.softmax(vs1.flatten())      
    vd = vd.numpy().astype('float32')
    
    vs1.tofile(f'build/{num}/src.bin')
    vd.tofile(f'build/{num}/golden.bin')
    vs1.tofile(f'src.bin')
    print
   

    return vd


def test(num, params, defs):
    h, w = params

    os.system(f"rm -rf build/{num} && mkdir -p build/{num}")

    golden = softmax(num, h, w)
    os.system(f"make DEFS='-DH={h} -DW={w} {defs}' run SIM={simulator} NUM={num} >build/{num}/test.log 2>&1")
    #os.system(f"make DEFS='-DH={h} -DW={w} {defs}' dump SIM={simulator} NUM={num}")

    result = from_txt( f'build/{num}/{simulator}.sig', golden, 0 )
    os.makedirs('check', exist_ok=True)
    check_result = check_to_txt( golden, result, f'check/{num}.data', 'np.allclose( result, golden, rtol=2e-3, atol=0, equal_nan=True)' )
    print(f"> {h}x{w}, check result: {check_result}")
    

if __name__ == "__main__":
    #############  h w
    params = (
            #( 1, 8 ),
            #( 4, 8 ),
            ( 16, 16),
            # ( 32, 8 ),
            )
    
    
    
    do_test(params, opt_levels, test, title, simulator, simulator!='spike')
