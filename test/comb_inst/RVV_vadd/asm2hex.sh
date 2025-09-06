#!/usr/bin/env bash
# 将 RISC-V 汇编(.s/.S)、目标文件(.o) 或 ELF(.elf) 转成十六进制表示。
# 处理要点：
# 1) 支持 RV64 (含压缩指令)，不再假设每条指令 4 字节。
# 2) 若输入是 .s 但本地汇编器不支持矩阵扩展，可直接使用已存在的同名 .o/.elf。
# 3) 生成三类输出：
#    - <base>.disasm   完整反汇编（含原始字节）
#    - <base>.hex      每行一条指令的原始字节（小端，去空格拼接）
#    - <base>.rawbytes 纯 .text 段顺序字节（逐字节一行）
# 用法示例： ./asm2hex.sh test.s  或  ./asm2hex.sh test.o  或  ./asm2hex.sh test.elf

set -euo pipefail

if [ $# -lt 1 ]; then
  echo "用法: $0 <file.s|file.o|file.elf>" >&2
  exit 1
fi

input="$1"
if [ ! -f "$input" ]; then
  echo "❌ 文件不存在: $input" >&2
  exit 1
fi

ext="${input##*.}"
base="${input%.*}"

# 可配置 ARCH（默认尝试包含向量与 zfh；矩阵扩展若汇编器不支持会忽略）
: "${ARCH:=rv64gcv_zfh}"
: "${ABI:=lp64d}"

# 选择工具
OBJDUMP=""
for t in llvm-objdump riscv64-unknown-elf-objdump; do
  if command -v "$t" >/dev/null 2>&1; then OBJDUMP="$t"; break; fi
done
if [ -z "$OBJDUMP" ]; then
  echo "❌ 未找到 objdump (llvm-objdump / riscv64-unknown-elf-objdump)" >&2
  exit 1
fi

OBJCOPY=""
for t in llvm-objcopy riscv64-unknown-elf-objcopy; do
  if command -v "$t" >/dev/null 2>&1; then OBJCOPY="$t"; break; fi
done
if [ -z "$OBJCOPY" ]; then
  echo "❌ 未找到 objcopy (llvm-objcopy / riscv64-unknown-elf-objcopy)" >&2
  exit 1
fi

CC=""
for t in riscv64-unknown-elf-gcc riscv64-unknown-elf-clang clang; do
  if command -v "$t" >/dev/null 2>&1; then CC="$t"; break; fi
done

OBJFILE=""
ELFFILE=""

case "$ext" in
  s|S)
    # 若已有同名 .o 或 .elf，优先复用避免因不支持矩阵扩展而失败
    if [ -f "${base}.o" ]; then
      OBJFILE="${base}.o"
    elif [ -f "${base}.elf" ]; then
      ELFFILE="${base}.elf"; OBJFILE="${base}.elf"
    else
      if [ -z "$CC" ]; then
        echo "⚠️ 未找到可用编译器，且无现成 ${base}.o；无法从汇编重新构建。" >&2
        exit 1
      fi
      echo "[*] 汇编 ${input} -> ${base}.o"
      set +e
      "$CC" -c "$input" -o "${base}.o" \
        -march="$ARCH" -mabi="$ABI" -menable-experimental-extensions 2>"${base}.asm.err"
      status=$?
      set -e
      if [ $status -ne 0 ]; then
        echo "⚠️ 汇编失败(可能是不支持的矩阵指令)。错误日志: ${base}.asm.err" >&2
        if [ ! -f "${base}.o" ]; then
          echo "❌ 无法生成对象文件，退出。" >&2
          exit 1
        fi
      fi
      OBJFILE="${base}.o"
    fi
    ;;
  o)
    OBJFILE="$input"
    ;;
  elf)
    ELFFILE="$input"; OBJFILE="$input"
    ;;
  *)
    echo "❌ 不支持的扩展名: .$ext (仅支持 .s .S .o .elf)" >&2
    exit 1
    ;;
esac

# 若是 ELF，保持；若是纯 .o 也按同样流程
[ -z "$ELFFILE" ] && ELFFILE="$OBJFILE"

if [ ! -f "$OBJFILE" ]; then
  echo "❌ 未找到对象文件: $OBJFILE" >&2
  exit 1
fi

echo "[*] 反汇编 -> ${base}.disasm"
"$OBJDUMP" -d "$OBJFILE" > "${base}.disasm"

echo "[*] 生成每指令十六进制 -> ${base}.hex"
# 解析 objdump 输出中的指令字节（适配 llvm-objdump 与 GNU 风格）
awk 'BEGIN{IGNORECASE=1}
  /^[[:space:]]*[0-9a-f]+:/ {
    line=$0
    sub(/^[[:space:]]*/,"", line)
    addr=substr(line,1, index(line,":")-1)
    sub(/^[0-9a-f]+:[[:space:]]*/,"", line)
    bytes=""
    # 把连续的 "hh" 或 "hh " 片段取出；遇到两个以上空格认为后面是助记符
    while (match(line, /^[0-9a-f]{2}([[:space:]]|$)/)) {
      b=substr(line, RSTART, 2)
      bytes=bytes b
      line=substr(line, RSTART+RLENGTH)
      # 去掉多余空格
      sub(/^[[:space:]]*/, "", line)
      # 如果后面不是继续字节序列就停
      if (match(line, /^[0-9a-f]{2}([[:space:]]|$)/)==0) break
    }
    if (bytes!="") print bytes
  }' "${base}.disasm" > "${base}.hex"

echo "[*] 导出 .text 原始字节 -> ${base}.rawbytes"
TMPBIN="${base}.text.bin"
"$OBJCOPY" -O binary --only-section=.text "$OBJFILE" "$TMPBIN"
hexdump -v -e '1/1 "%02x\n"' "$TMPBIN" > "${base}.rawbytes"
rm -f "$TMPBIN"

echo "✅ 完成："
echo "    反汇编: ${base}.disasm"
echo "    指令HEX: ${base}.hex   (每行一条指令的字节串；含压缩指令长度可变)"
echo "    原始字节: ${base}.rawbytes (逐字节)"
if [ -f "${base}.asm.err" ]; then
  echo "    ⚠️ 汇编日志: ${base}.asm.err (若有指令不被识别可参考)"
fi