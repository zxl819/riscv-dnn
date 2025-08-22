CLANG := clang
SPIKE := spike
PK := pk

objects = test.cpp mat.cpp

target = rvv-test

$(target) : $(objects)
	$(CLANG)  --target=riscv64-unknown-elf -menable-experimental-extensions -march=rv64gcv1p0 --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -o $(target) $(objects)

run :
	$(SPIKE) --isa=rv64gcv $(PK) $(target)

clean :
	rm -f $(target)

# --- Matrix tile test: build assembly only ---
.PHONY: asm-tilem
asm-tilem: build/test_tilem.s

build/test_tilem.s: test_tilem.c matmul_rvm_tile.h ../include/matrix/matrix_intrinsic.h
	@mkdir -p build
	$(CLANG) --target=riscv64-unknown-elf -S \
	  -menable-experimental-extensions \
	  -march=rv64gcv0p10_zfh0p1 \
	  -D__riscv_matrix=1 -DENABLE_RVM_MATRIX=1 \
	  -O2 -ffp-contract=fast \
	  -I../include \
	  test_tilem.c -o $@