NUM ?= 0


# top_dir = ../../..
top_dir := /nfs/home/zhengsihan/Code/new-tests/src-tests/riscv-dnn
inc_dir = $(top_dir)/include
src_dir = $(top_dir)/src

includes = -I$(inc_dir)/env -I$(inc_dir)/common -I$(src_dir) -Ibuild/$(NUM)

# 宏定义
defines = $(DEFS)

SHELL := /usr/bin/bash

RISCV_HOME=/nfs/home/zhengsihan/opt/riscv
SYSROOT = "$(RISCV_HOME)/riscv64-unknown-elf"


SIM := spike
# toolchains
# supported: llvm gnu
TC ?= llvm

NCORES := 1

SPIKE := spike
SPIKE_ARGS :=



ifeq (x$(SIM), xspike)
	SIM_CMD ?= \
		$(SPIKE)  --isa=rv64gcv_zfh_zvfh_matrix \
			+signature=build/$(NUM)/spike.sig +signature-granularity=32 
	SIMV_POST := > build/$(NUM)/spike.log 2>&1
	defines += -D__SPIKE__
endif


PREFIX ?= riscv64-unknown-elf-
OBJDUMP := $(PREFIX)objdump


CC := $(RISCV_HOME)/llvm-martix/bin/clang
CFLAGS := 	--sysroot=$(SYSROOT)  \
		-I$(RISCV_HOME)/llvm-withlib/include/c++/v1/ \
		-I$(RISCV_HOME)/riscv64-unknown-elf/riscv64-unknown-elf/include/ \
		--target=riscv64-unknown-elf -march=rv64gv0p10zfh0p1 \
		-menable-experimental-extensions -DPREALLOCATE=1 \
		-mcmodel=medany -static -std=gnu99 -O2 -ffast-math \
		-fno-common -fno-builtin-printf -mabi=lp64d  -nostdlib -nostartfiles  "$1" ${2:+$2} ${3:+$3} ${4:+$4}
LINK:= $(RISCV_HOME)/riscv64-unknown-elf/bin/riscv64-unknown-elf-gcc   
	
LDFLAGS := 	--sysroot=$(SYSROOT)  \
			-L$(RISCV_HOME)/riscv64-unknown-elf/lib \
			-L$(RISCV_HOME)/riscv64-unknown-elf/lib/gcc/riscv64-unknown-elf/13.2.0 \
			-L$(RISCV_HOME)/riscv64-unknown-elf/riscv64-unknown-elf/lib \
			-L$(RISCV_HOME)/lib/gcc/lib  \
			-static  -nostdlib -nostartfiles -lm -lgcc \
			-T $(inc_dir)/common/test.ld 


target_elf = build/$(NUM)/test.elf
target_dump = build/$(NUM)/test.dump
target_map = build/$(NUM)/test.map

objects = build/$(NUM)/test.o crt.o syscalls.o

all: $(target_elf)

syscalls.o: $(inc_dir)/common/syscalls.c
	$(CC) $(CFLAGS) -c -o $@ $<

crt.o: $(inc_dir)/common/crt.S
	$(CC) $(CFLAGS) -c -o $@ $<

build/$(NUM)/test.o: test.c
	$(CC) $(CFLAGS) -c -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<
	
$(target_elf): $(objects)
	$(LINK) -o $(target_elf) $^ $(LDFLAGS)

$(target_map): $(target_elf)
	$(PREFIX)readelf -s -W $< > $@

run: $(target_elf) $(target_map)
	$(SIM_CMD)$(target_elf) $(SIMV_POST)

dump: $(target_dump)

$(target_dump): $(target_elf)
	$(OBJDUMP) -S $(target_elf) > $(target_dump)
	
clean:
	rm -f $(target_elf) $(objects) $(target_dump) $(target_map) *.sig

