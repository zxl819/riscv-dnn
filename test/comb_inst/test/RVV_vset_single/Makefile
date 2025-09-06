include ../../../common-2xl-backup.mk

# 生成 test.c 的汇编文件：build/$(NUM)/test.s
# 用法：make NUM=1 ENABLE_RVM_MATRIX=1 asm
asm: build/$(NUM)/test.s

build/$(NUM)/test.s: test.c
	@mkdir -p $(dir $@)
	$(TEST_C_COMPILER) $(TEST_C_FLAGS) -S -fverbose-asm -o $@ $<
	@echo "[ASM] $@ generated"
