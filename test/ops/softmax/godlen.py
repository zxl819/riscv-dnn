# import os
# import sys
# import numpy as np
# import pandas as pd

import numpy as np

vs1 = np.array([
    -0.50183952, 1.80285728, 0.92797577, 0.39463395,
    -1.37592542, -1.37602186, -1.76766551, 1.46470463,
    0.40446004, 0.83229029, -1.91766202, 1.87963939,
    1.32977057, -1.15064359, -1.27270019, -1.26638198
], dtype=np.float32)

def softmax(x: np.ndarray) -> np.ndarray:
    x = x.astype(np.float32)
    m = np.max(x)
    e = np.exp(x - m)
    return (e / np.sum(e)).astype(np.float32)

vd = softmax(vs1)
print("golden:", vd)

