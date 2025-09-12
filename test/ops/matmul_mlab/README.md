# matmul 测试说明

简要：本测试在 RISC‑V 目标上执行矩阵乘法 dst = src1 × src2（数据为 float16）。输入二进制嵌入到可执行文件，输出写入 ELF 的 `.scdata.output` 段（符号名为 `dstData`）。

## 文件说明
- `test.c`：测试入口，调用 `matmul(&dst, &src1, &src2)`。输入数据通过 `INCBIN(src1.bin)`、`INCBIN(src2.bin)` 嵌入。输出缓冲 `dstData` 位于段 `.scdata.output`。
- `params.h`：默认维度（可修改）：
  - `M = 16384`
  - `K = 300`
  - `N = 300`
  - `OUT_SIZE = M * N`
  - `NLOOPS = 1`（循环次数）
- `src1.bin`, `src2.bin`：输入二进制文件（每元素 float16，按行主序）；位于本目录或由生成脚本/上层流程准备。
- `cpi.h`：性能统计宏（使用 mcycle/minstret CSR，打印 cycles/instructions/CPI）。
- 生成产物：`build/$(NUM)/test.elf`，以及可通过 objcopy 导出的 `.scdata.output` 二进制。

## 数据格式与布局
- 数据类型：float16（IEEE 754 半精度），每元素占 2 字节（little-endian）。
- src1 矩阵：维度 M × K，按行主序连续存储（共 M*K 元素，文件大小 = M*K*2 字节）。
- src2 矩阵：维度 K × N，按行主序（共 K*N 元素）。
- dst 矩阵（输出）：维度 M × N，按行主序（共 M*N 元素），位于 ELF 段 `.scdata.output`。

## 编译与运行（交叉 RISC‑V 环境）
构建：
```bash
cd /mnt/d/riscv-dnn/test/ops/matmul
make NUM=0
```
运行（使用 Makefile 的 run 目标，会调用已配置的模拟器）：
```bash
make NUM=0 run
```
构建产物位于 `build/0/`（取决于 `NUM`）。

## 导出输出二进制
使用交叉 objcopy 将 ELF 中的 `.scdata.output` 段导出为 `output.bin`：
```bash
# 根据你的工具链前缀调整下面命令
$RISCV_TOOLCHAIN_PREFIXobjcopy --dump-section .scdata.output=output.bin build/0/test.elf
# 例如：
~/opt/riscv/bin/riscv64-unknown-elf-objcopy --dump-section .scdata.output=output.bin build/0/test.elf
```
`output.bin` 即为按行主序的 float16 数组，元素个数 = M*N。

## 本地验证（Python / NumPy）
下面示例脚本用于在主机上读取输入和导出的输出，计算参考值并比较误差。

```python
# validate.py
import numpy as np

M = 16384  # 与 params.h 保持一致
K = 300
N = 300

# 读取输入（如果本地有 src1.bin、src2.bin）
a = np.fromfile('src1.bin', dtype=np.float16).reshape(M, K)
b = np.fromfile('src2.bin', dtype=np.float16).reshape(K, N)

# 读取目标输出（从 ELF 导出的 output.bin）
out = np.fromfile('output.bin', dtype=np.float16).reshape(M, N)

# 计算参考（使用 float32 精度以减少舍入误差）
gold = (a.astype(np.float32) @ b.astype(np.float32)).astype(np.float32)

# 比较
absdiff = np.abs(out.astype(np.float32) - gold)
print('max abs diff =', absdiff.max())
print('mean abs diff =', absdiff.mean())

# 简单通过判定示例
TOL = 1e-2
print('PASS' if absdiff.max() <= TOL else 'FAIL')
```

期望误差阈值：由于 float16 精度限制，典型可接受 `max_abs_diff < 1e-2`（根据你的精度要求可调整）。

## 参数调整
- 修改维度：编辑 `params.h` 中的 `M/K/N` 值，或在编译前修改宏并重新准备对应的 `src1.bin`/`src2.bin`。
- 循环次数：通过 `NLOOPS` 控制迭代次数以做性能统计。
- DEBUG：若启用源码中 `DEBUG_PRINT`（编译时定义），会打印更多调试信息。

## 期望运行输出
- 程序控制台会打印：
  - `Begin`
  - （性能统计）如：`<code>: 123456 cycles,7890 instructions, 1234.5 cycles/iter, 15.6 CPI`
  - `End`

## 故障排查要点
- 头文件找不到（例如 `riscv_matrix.h` 或 `<array>`）：确认交叉工具链（`RISCV_HOME`）是否完整并包含 libc++ headers，或在 `common-2xl.mk` 中把项目 `include` 路径加入 `-I`。
- ELF 中未出现 `.scdata.output`：检查链接脚本 `include/common/test.ld` 是否保留该段；`dstData` 是否被正确放到该段。
- 数值异常：检查输入二进制是否为 float16 且字节序为 little-endian；确认 `src1.bin`/`src2.bin` 的尺寸与 `params.h` 中的 M/K/N 对应。

---

如果你希望，我可以：
- 把上述 `validate.py` 文件直接添加到本目录并在 README 中提供具体调用示例；
- 或者根据你指定的误差容限将 README 中的通过判定阈值调整为更严格或更宽松的值。

请告诉我是否要我同时创建 `validate.py`，以及你希望使用的误差容限。
