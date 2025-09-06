```makefile
top_dir := /mnt/d/for_test
inc_dir = $(top_dir)/include
#src_dir = $(top_dir)/src

RV_MARCH ?= rv64gcv1p00_zfh0p1

\# 是否启用 Matrix 扩展实现（RVM）。0=默认关闭（使用 RVV 实现）；1=开启（使用 RVM 实现）

ENABLE_RVM_MATRIX ?= 0

\# 编译公共 C/C++ 选项，使用-O1优化

CFLAGS := \

  --sysroot=$(SYSROOT) \

  -I$(RISCV_HOME)/include \

  -I$(SYSROOT)/include \

  $(if $(and $(LIBCXX_ENABLE),$(wildcard $(LIBCXX_INCLUDEDIR))),-I$(LIBCXX_INCLUDEDIR)) \

  -I$(RISCV_HOME)/riscv64-unknown-elf/riscv64-unknown-elf/include/ \

  --target=riscv64-unknown-elf -march=$(RV_MARCH) \

  -menable-experimental-extensions -DPREALLOCATE=1 \

  -mno-relax \

  -mcmodel=medany -static -std=gnu99 -O1 -ffast-math \

  -fno-common -fno-builtin-printf -nostdlib -nostartfiles

CFLAGS += $(includes) -I$(top_dir)/include

LDSCRIPT := $(inc_dir)/common/test.ld


objects = build/$(NUM)/test.o crt.o syscalls.o syscall_stubs.o

# 启动文件编译
build/$(NUM)/start.o: $(STARTUP)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# # 链接时加入启动文件
build/$(NUM)/test.elf: build/$(NUM)/test.o build/$(NUM)/start.o
	$(CC) $(LDFLAGS) -T $(LDSCRIPT) $^ -o $@

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
.c.o:
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(target_elf): $(objects)
	$(LINK) -o $(target_elf) $^ $(LDFLAGS)

$(target_map): $(target_elf)
	$(PREFIX)readelf -s -W $< > $@

run: $(target_elf) $(target_map)
	$(SIM_CMD) $(target_elf) $(SIMV_POST)

dump: $(target_dump)

$(target_dump): $(target_elf)
	$(OBJDUMP) -S $(target_elf) > $(target_dump)
```

