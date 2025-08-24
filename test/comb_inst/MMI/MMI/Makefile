include ../../../../common-2xl.mk

asm: build/$(NUM)/test.s
# make NUM=2 ENABLE_RVM_MATRIX=1 asm
build/$(NUM)/test.s: test.c
	@mkdir -p $(dir $@)
	$(TEST_C_COMPILER) $(TEST_C_FLAGS) -S -fverbose-asm -o $@ $<
	@echo "[ASM] $@ generated"