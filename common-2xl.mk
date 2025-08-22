NUM ?= 0


# top_dir = ../../..
top_dir := /mnt/d/riscv-dnn
inc_dir = $(top_dir)/include
src_dir = $(top_dir)/src
# includes/defines：统一追加的 -I 与 -D。
includes = -I$(inc_dir)/env -I$(inc_dir)/common -I$(src_dir) -Ibuild/$(NUM)

# 宏定义 交叉工具链与 newlib sysroot。
defines = $(DEFS)

SHELL := /usr/bin/bash

RISCV_HOME := $(HOME)/opt/riscv
SYSROOT := $(RISCV_HOME)/riscv64-unknown-elf

# 一键开关：启用本地安装的 libc++/libc++abi/libunwind
# 头文件位于 $(LIBCXX_PREFIX)/include/c++/v1
# 库位于 $(LIBCXX_PREFIX)/lib
LIBCXX_ENABLE ?= 1
LIBCXX_PREFIX ?= $(RISCV_HOME)/llvm-withlib
LIBCXX_INCLUDEDIR := $(LIBCXX_PREFIX)/include/c++/v1
LIBCXX_LIBDIR := $(LIBCXX_PREFIX)/lib

# 模拟器/运行环境集成
SIM := spike
# toolchains
# supported: llvm gnu
# 工具链选择与前端编译器
TC ?= llvm

NCORES := 1

SPIKE := spike
SPIKE_ARGS :=



ifeq (x$(SIM), xspike)
SIM_CMD ?= \
	$(SPIKE) --isa=rv64gcv_zfh_zvfh_matrix \
		+signature=build/$(NUM)/spike.sig +signature-granularity=32
SIMV_POST := > build/$(NUM)/spike.log 2>&1
defines += -D__SPIKE__
endif


PREFIX ?= riscv64-unknown-elf-
OBJDUMP := $(PREFIX)objdump

# CLANG_HOME 设定后 CC / CXX 指向自编 LLVM。
CLANG_HOME := $(HOME)/opt/riscv/riscv-matrix-project/llvm-project
CC := $(CLANG_HOME)/build-ninja/bin/clang
CXX := $(CLANG_HOME)/build-ninja/bin/clang++

# RISC-V 架构字符串（可覆盖）。
# 说明：Clang 14 仍将 V 扩展视作实验特性，需要显式版本号（如 v0p10）。
# 如使用较新编译器（已支持稳定 V 扩展），可覆盖为 rv64gcv_zfh_zvfh。
RV_MARCH ?= rv64gcv0p10_zfh0p1
# 是否启用 Matrix 扩展实现（RVM）。0=默认关闭（使用 RVV 实现）；1=开启（使用 RVM 实现）
ENABLE_RVM_MATRIX ?= 0
# 编译公共 C/C++ 选项
CFLAGS := \
	--sysroot=$(SYSROOT) \
	-I$(RISCV_HOME)/include \
	-I$(SYSROOT)/include \
	$(if $(and $(LIBCXX_ENABLE),$(wildcard $(LIBCXX_INCLUDEDIR))),-I$(LIBCXX_INCLUDEDIR)) \
	-I$(RISCV_HOME)/riscv64-unknown-elf/riscv64-unknown-elf/include/ \
	--target=riscv64-unknown-elf -march=$(RV_MARCH) \
	-menable-experimental-extensions -DPREALLOCATE=1 \
	-mno-relax \
	-mcmodel=medany -static -std=gnu99 -O2 -ffast-math \
	-fno-common -fno-builtin-printf -mabi=lp64d  -nostdlib -nostartfiles
CFLAGS += $(includes) -I$(top_dir)/include

# 根据是否启用 RVM，调整宏定义（默认使用 stub；启用 RVM 则使用真实头并切换实现）
ifeq ($(ENABLE_RVM_MATRIX),1)
CFLAGS += -DENABLE_RVM_MATRIX -U RVDNN_FORCE_STUB_MATRIX -D__riscv_matrix=1
else
CFLAGS += -DRVDNN_FORCE_STUB_MATRIX=1
endif

ifeq ($(and $(LIBCXX_ENABLE),$(wildcard $(LIBCXX_INCLUDEDIR))),)
$(warning "libc++ headers not found under $(LIBCXX_INCLUDEDIR); C++ headers like <array> may be missing for cross-compilation.")
endif
# 说明：当前 clang-14 不接受在 -march 中追加 "_matrix"，故不自动拼接；
# 若你的编译器支持，也可手动覆盖 RV_MARCH 自行追加。
# 使用 clang++ 作为链接驱动，避免依赖缺失的 GNU gcc 可执行文件
LINK := $(CXX)
	

# 可选紧凑布局：避免原 test.ld 巨大 ALIGN 造成 500MB 级文件
LAYOUT_COMPACT ?= 0
ifeq ($(LAYOUT_COMPACT),1)
LDSCRIPT := $(inc_dir)/common/test_compact.ld
else
LDSCRIPT := $(inc_dir)/common/test.ld
endif

# 基础链接 flags（裸机）
LDFLAGS := \
	--sysroot=$(SYSROOT) \
	--target=riscv64-unknown-elf -march=$(RV_MARCH) -mabi=lp64d \
	-L$(RISCV_HOME)/riscv64-unknown-elf/lib \
	-L$(RISCV_HOME)/riscv64-unknown-elf/riscv64-unknown-elf/lib \
	-static -nostdlib -nostartfiles -lm \
	-Wl,--no-relax \
	-fuse-ld=lld -rtlib=compiler-rt -unwindlib=libunwind \
	-T $(LDSCRIPT)

# 启用 libc++ 时追加的链接 flags
ifeq ($(LIBCXX_ENABLE),1)
LDFLAGS += \
			-L$(LIBCXX_LIBDIR) \
			-Wl,--start-group -lc++ -lc++abi -lunwind -Wl,--end-group \
			-lc
endif

# C++ 编译选项（与构建的 libc++ 配置保持一致：无异常/RTTI，显式 C++ 头）
CXXFLAGS := \
	--sysroot=$(SYSROOT) \
	-I$(RISCV_HOME)/include \
	-I$(SYSROOT)/include \
	$(if $(and $(LIBCXX_ENABLE),$(wildcard $(LIBCXX_INCLUDEDIR))),-I$(LIBCXX_INCLUDEDIR)) \
	-I$(RISCV_HOME)/riscv64-unknown-elf/riscv64-unknown-elf/include/ \
	--target=riscv64-unknown-elf -march=$(RV_MARCH) \
	-menable-experimental-extensions -DPREALLOCATE=1 \
	-mno-relax \
	-mcmodel=medany -static -std=c++20 -O2 -ffast-math \
	-fno-exceptions -fno-rtti -fno-common -fno-builtin-printf -mabi=lp64d \
	-D_LIBCPP_HAS_NO_LIBRARY_ALIGNED_ALLOCATION -D_LIBCPP_HAS_NO_ALIGNED_ALLOCATION -D_LIBCPP_HAS_NO_C11_ALIGNED_ALLOC \
	-D_LIBCPP_HAS_NO_MONOTONIC_CLOCK -D_LIBCPP_ENABLE_ASSERTIONS=0 \
	-nostdlib -nostartfiles -nostdinc++
CXXFLAGS += $(includes) -I$(top_dir)/include

ifeq ($(ENABLE_RVM_MATRIX),1)
CXXFLAGS += -DENABLE_RVM_MATRIX -U RVDNN_FORCE_STUB_MATRIX -D__riscv_matrix=1
else
CXXFLAGS += -DRVDNN_FORCE_STUB_MATRIX=1
endif
# 目标与产物命名
target_elf = build/$(NUM)/test.elf
target_dump = build/$(NUM)/test.dump
target_map = build/$(NUM)/test.map
objects = build/$(NUM)/test.o crt.o syscalls.o
# - syscalls.o / crt.o：单独规则编译启动与系统调用封装。
# - test.o：根据是否启用 libc++ 选择 C 或 C++ 编译器（TEST_C_COMPILER / TEST_C_FLAGS）
all: $(target_elf)

syscalls.o: $(inc_dir)/common/syscalls.c
	$(CC) $(CFLAGS) -c -o $@ $<

crt.o: $(inc_dir)/common/crt.S
	$(CC) $(CFLAGS) -c -o $@ $<

# 在启用 libc++ 时，允许测试主文件以 C++ 模式编译（因包含 <array> 等 C++ 头）
ifeq ($(LIBCXX_ENABLE),1)
TEST_C_COMPILER := $(CXX)
TEST_C_FLAGS := $(CXXFLAGS)
else
TEST_C_COMPILER := $(CC)
TEST_C_FLAGS := $(CFLAGS)
endif

build/$(NUM)/test.o: test.c
	@mkdir -p $(dir $@)
	$(TEST_C_COMPILER) $(TEST_C_FLAGS) -c -o $@ $<

.c.o:
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

# C++ 源文件规则
.cc.o:
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.cpp.o:
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.cxx.o:
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	
$(target_elf): $(objects)
	$(LINK) -o $(target_elf) $^ $(LDFLAGS)

$(target_map): $(target_elf)
	$(PREFIX)readelf -s -W $< > $@

run: $(target_elf) $(target_map)
	$(SIM_CMD) $(target_elf) $(SIMV_POST)

dump: $(target_dump)

$(target_dump): $(target_elf)
	$(OBJDUMP) -S $(target_elf) > $(target_dump)
	
clean:
	rm -f $(target_elf) $(objects) $(target_dump) $(target_map) *.sig

