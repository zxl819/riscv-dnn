#!/usr/bin/env bash
set -euo pipefail

# 推荐 Python 版本：3.7/3.8/3.9。若系统没有，请先安装（sudo apt install python3.8 python3.8-venv）。
#PYTHON=${PYTHON:-python3.8}
VENV_DIR=${VENV_DIR:-$HOME/.venv/riscv-dnn}
PYTHON=python3 
REQ_FILE=/mnt/d/riscv-dnn/requirement.txt 
#REQ_FILE=${REQ_FILE:-requirement.txt}

echo "使用 Python: $PYTHON"
"$PYTHON" -V

# 创建并激活虚拟环境
if [ ! -d "$VENV_DIR" ]; then
  echo "创建虚拟环境: $VENV_DIR"
  "$PYTHON" -m venv "$VENV_DIR"
fi

# 激活并升级工具
# 注意：在脚本内激活只对此脚本子shell有效，脚本执行完成后请在交互 shell 中手动 source 激活
source "$VENV_DIR/bin/activate"
pip install --upgrade pip setuptools wheel

# 安装系统依赖（如果需要编译某些包）
# sudo apt update && sudo apt install -y build-essential libssl-dev libffi-dev libblas-dev liblapack-dev

# 使用 requirement.txt 安装
if [ ! -f "$REQ_FILE" ]; then
  echo "未找到 $REQ_FILE，退出" >&2
  exit 1
fi

# 尝试直接安装全部依赖
pip install -r "$REQ_FILE" --no-cache-dir || {
  echo "pip 安装失败，尝试分步安装常见问题包（tensorflow / jax / jaxlib）"
  # 安装 numpy/pandas/scipy 等基础包后再安装大包
  pip install numpy pandas scipy matplotlib --no-cache-dir
  # TensorFlow 2.4.0 对应 Python 3.8 最稳定；若需要 CPU 版本：
  pip install tensorflow==2.4.0 --no-cache-dir || echo "tensorflow 安装失败，请检查 Python 版本或使用官方 wheel 指定"
  # jax/jaxlib CPU 版本（可替换为合适的 wheel URL）
  pip install jax==0.2.17 jaxlib==0.1.69 --no-cache-dir || echo "jax/jaxlib 安装失败，请参照 JAX 官方安装说明"
  # 安装其余依赖
  pip install -r "$REQ_FILE" || true
}

echo "完成。要在当前 shell 使用虚拟环境："
echo "  source $VENV_DIR/bin/activate"
echo "验证： python -c \"import numpy,pandas; import tensorflow as tf; print(numpy.__version__, pandas.__version__, tf.__version__)\""