	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0_v0p10_zfh0p1_zvlsseg0p10"
	.file	"test_tilem.c"
	.globl	main
	.p2align	1
	.type	main,@function
main:
	mv	a0, zero
	addi	a1, zero, 100
.LBB0_1:
	mv	a2, zero
	subw	a3, a1, a0
	slli	a3, a3, 32
	srli	a3, a3, 32
	msettilem	a6, a3
.LBB0_2:
	mv	a4, zero
	subw	a5, a1, a2
	slli	a5, a5, 32
	srli	a5, a5, 32
	msettilen	a5, a5
.LBB0_3:
	subw	a3, a1, a4
	slli	a3, a3, 32
	srli	a3, a3, 32
	msettilek	a3, a3
	addw	a4, a4, a3
	blt	a4, a1, .LBB0_3
	addw	a2, a2, a5
	blt	a2, a1, .LBB0_2
	addw	a0, a0, a6
	blt	a0, a1, .LBB0_1
	mv	a0, zero
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main

	.section	".linker-options","e",@llvm_linker_options
	.ident	"clang version 14.0.0 (git@github.com:riscv-stc/llvm-project.git 70d1e5ec4b9a3dcaf4e8731c83333c5f98618e27)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
