#!/bin/bash

# 打包脚本：收集指定测试的 build 文件（仅 .o/.s/.dump/.elf）和相关源代码
# 用法：./pack_build.sh TEST_TYPE TEST_NAME [NUM] [LLVM_VERSION]
# 示例：./pack_build.sh ops matmul_mlab 20 14
# TEST_TYPE：测试类型文件夹名（如 ops）
# TEST_NAME：测试文件夹名（如 matmul_mlab）
# NUM：build 子文件夹名，默认 0
# LLVM_VERSION：LLVM 版本，默认空（用于文件名）

# 检查参数
if [ $# -lt 2 ]; then
    echo "用法：./pack_build.sh TEST_TYPE TEST_NAME [NUM] [LLVM_VERSION]"
    echo "示例：./pack_build.sh ops matmul_mlab 20 14"
    exit 1
fi

TEST_TYPE=$1
TEST_NAME=$2
NUM=${3:-0}  # 默认 NUM=0
LLVM_VERSION=${4:-""}  # 默认空

BUILD_DIR="/mnt/d/riscv-dnn/test/${TEST_TYPE}/${TEST_NAME}/build/${NUM}"
OUTPUT_FILE="${BUILD_DIR}/Clang${LLVM_VERSION}_${TEST_NAME}_build_${NUM}.zip"

# 检查 zip 命令是否存在
if ! command -v zip &> /dev/null; then
    echo "错误：zip 命令未安装。请运行 'sudo apt install zip' 或 'sudo yum install zip' 安装。"
    exit 1
fi

# 检查 build 目录是否存在
if [ ! -d "$BUILD_DIR" ]; then
    echo "错误：build 目录 $BUILD_DIR 不存在。请先运行 make。"
    exit 1
fi

# 创建临时目录用于收集文件
TEMP_DIR="temp_pack"
mkdir -p "$TEMP_DIR"

# 收集 build 文件
# echo "收集 build 文件..."
#  cp -r "$BUILD_DIR"/* "$TEMP_DIR/" 2>/dev/null || echo "警告：build 目录为空或无文件"
# 收集 build 文件（仅 .o/.s/.dump/.elf）
echo "收集 build 文件（仅 .o/.s/.dump/.elf）..."
find "$BUILD_DIR" -name "*.o" -o -name "*.s" -o -name "*.dump" -o -name "*.elf"  -o -name "*.h"  -o -name "*.log"| xargs -I {} cp {} "$TEMP_DIR/" 2>/dev/null || echo "警告：没有找到匹配的文件"


# 收集测试相关的源文件
echo "收集源文件..."
cp "test/${TEST_TYPE}/${TEST_NAME}/test.c" "$TEMP_DIR/" 2>/dev/null || echo "警告：test/${TEST_TYPE}/${TEST_NAME}/test.c 不存在"
cp "test.c" "$TEMP_DIR/" 2>/dev/null || echo "警告：根目录 test.c 不存在（可选）"

# 收集配置文件（可选，保持通用）
cp common-2xl.mk "$TEMP_DIR/" 2>/dev/null || echo "警告：common-2xl.mk 不存在"
cp include/link.ld "$TEMP_DIR/" 2>/dev/null || echo "警告：link.ld 不存在"
# cp src/matmul_rvm_int8.h "$TEMP_DIR/" 2>/dev/null || echo "警告：matmul_rvm_int8.h 不存在"
cp include/riscv_matrix.h "$TEMP_DIR/" 2>/dev/null || echo "警告：riscv_matrix.h 不存在"

# 打包成 zip
echo "创建压缩包 $OUTPUT_FILE..."
zip -j "$OUTPUT_FILE" "$TEMP_DIR"/*  # zip 需要 -r 递归

# 清理临时目录
rm -rf "$TEMP_DIR"

# 输出信息
echo "打包完成！"
echo "压缩包：$OUTPUT_FILE"
echo "包含文件："
unzip -l "$OUTPUT_FILE" | head -20  # 显示前 20 个文件
echo "... (更多文件请查看 $OUTPUT_FILE)"

# 可选：显示文件大小
du -h "$OUTPUT_FILE"