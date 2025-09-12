import numpy as np
import sys

# Default sizes; keep in sync with params.h
M = 16384
K = 300
N = 300

src1_path = 'src1.bin'
src2_path = 'src2.bin'
out_path = 'output.bin'

if len(sys.argv) >= 4:
    src1_path, src2_path, out_path = sys.argv[1:4]

# Load inputs
a = np.fromfile(src1_path, dtype=np.float16).reshape(M, K)
b = np.fromfile(src2_path, dtype=np.float16).reshape(K, N)
# Load DUT output
out = np.fromfile(out_path, dtype=np.float16).reshape(M, N)

# Reference in float32 for better accuracy
ref = (a.astype(np.float32) @ b.astype(np.float32)).astype(np.float32)

absdiff = np.abs(out.astype(np.float32) - ref)
print('max abs diff =', float(absdiff.max()))
print('mean abs diff =', float(absdiff.mean()))

TOL = 1e-2
print('PASS' if absdiff.max() <= TOL else 'FAIL')
