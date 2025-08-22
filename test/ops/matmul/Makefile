include ../../../common-2xl.mk
# ---------------- Tile Matmul Test (test_tilem.c) ----------------
# 自动生成 src1.bin / src2.bin -> 编译 test_tilem.c -> 生成 build/$(NUM)/test_tilem.elf 并可运行

.PHONY: tile tile-run tile-clean

TILE_ELF := build/$(NUM)/test_tilem.elf
TILE_OBJ := build/$(NUM)/test_tilem.o

# 数据生成：依赖环境变量 M K N（默认 100）
src1.bin src2.bin:
	@echo "[GEN] float16 src1.bin/src2.bin (M=$${M:-100} K=$${K:-100} N=$${N:-100})"
	@python3 -c 'import os, numpy as np; M=int(os.environ.get("M",100));K=int(os.environ.get("K",100));N=int(os.environ.get("N",100));\
import sys; vs1=(np.random.rand(M,K).astype("float16")*2-1); vs2=(np.random.rand(K,N).astype("float16")*2-1);\
vs1.tofile("src1.bin"); vs2.tofile("src2.bin"); print(f"Wrote src1.bin({{vs1.nbytes}}B) src2.bin({{vs2.nbytes}}B)")'

# 对应对象文件（依赖生成的数据以满足 .incbin）
$(TILE_OBJ): test_tilem.c src1.bin src2.bin
	@mkdir -p $(dir $@)
	$(TEST_C_COMPILER) $(TEST_C_FLAGS) -c -o $@ $<

$(TILE_ELF): $(TILE_OBJ) crt.o syscalls.o
	$(LINK) -o $@ $^ $(LDFLAGS)

tile: $(TILE_ELF)

tile-run: $(TILE_ELF)
	$(SIM_CMD) $(TILE_ELF) $(SIMV_POST)
	@if [ "x$(SIM)" = "xgem5" ]; then \
		if [ -f build/$(NUM)/spike.sig ]; then mv build/$(NUM)/spike.sig build/$(NUM)/tilem.sig; fi; \
		if [ -f build/$(NUM)/spike.log ]; then mv build/$(NUM)/spike.log build/$(NUM)/tilem.log; fi; \
		true; \
	fi

tile-clean:
	rm -f $(TILE_ELF) $(TILE_OBJ)

# Build test_tilem.c into a standalone bare-metal ELF using Matrix extension (separate simple variant)
.PHONY: tile-elf simple-tile-elf

SIMPLE_TILE_OUT := build_tile
SIMPLE_TILE_ELF := $(SIMPLE_TILE_OUT)/test.elf
SIMPLE_TILE_OBJS := $(SIMPLE_TILE_OUT)/crt.o $(SIMPLE_TILE_OUT)/syscalls.o $(SIMPLE_TILE_OUT)/dso.o $(SIMPLE_TILE_OUT)/test.o

tile-elf: simple-tile-elf

simple-tile-elf: $(SIMPLE_TILE_ELF)

$(SIMPLE_TILE_OUT)/crt.o: $(inc_dir)/common/crt.S
	@mkdir -p $(SIMPLE_TILE_OUT)
	$(CC) $(CFLAGS) -c -o $@ $<

$(SIMPLE_TILE_OUT)/syscalls.o: $(inc_dir)/common/syscalls.c
	@mkdir -p $(SIMPLE_TILE_OUT)
	$(CC) $(CFLAGS) -c -o $@ $<

$(SIMPLE_TILE_OUT)/dso.o: $(inc_dir)/common/dso.c
	@mkdir -p $(SIMPLE_TILE_OUT)
	$(CC) $(CFLAGS) -c -o $@ $<

$(SIMPLE_TILE_OUT)/test.o: test_tilem.c $(top_dir)/src/matmul_tile.h src1.bin src2.bin
	@mkdir -p $(SIMPLE_TILE_OUT)
	$(CC) $(CFLAGS) -c -o $@ $<

$(SIMPLE_TILE_ELF): $(SIMPLE_TILE_OBJS)
	$(LINK) -o $@ $^ $(LDFLAGS)
