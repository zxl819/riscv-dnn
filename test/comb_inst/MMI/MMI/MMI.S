	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0_m2p0_a2p0_f2p0_d2p0_v0p10_zfh0p1_zvlsseg0p10"
	.file	"MMI.c"
	.globl	main                            # -- Begin function main
	.p2align	2
	.type	main,@function
main:                                   # @main
# %bb.0:
	addi	sp, sp, -48
	sd	ra, 40(sp)                      # 8-byte Folded Spill
	sd	s0, 32(sp)                      # 8-byte Folded Spill
	sd	s1, 24(sp)                      # 8-byte Folded Spill
	sd	s2, 16(sp)                      # 8-byte Folded Spill
	sd	s3, 8(sp)                       # 8-byte Folded Spill
	sd	s4, 0(sp)                       # 8-byte Folded Spill
	#APP
	csrr	a1, mlenb
	#NO_APP
	lui	a0, %hi(.L.str)
	addi	a0, a0, %lo(.L.str)
	sext.w	a1, a1
	call	printf
	#APP
	csrr	a1, mrlenb
	#NO_APP
	lui	a0, %hi(.L.str.1)
	addi	a0, a0, %lo(.L.str.1)
	sext.w	a1, a1
	call	printf
	#APP
	csrr	a0, mtype
	#NO_APP
	lui	a1, %hi(.L.str.2)
	addi	s2, a1, %lo(.L.str.2)
	sext.w	a1, a0
	mv	a0, s2
	call	printf
	#APP
	csrr	a0, mtilem
	#NO_APP
	lui	a1, %hi(.L.str.3)
	addi	s1, a1, %lo(.L.str.3)
	sext.w	a1, a0
	mv	a0, s1
	call	printf
	#APP
	csrr	a0, mtilen
	#NO_APP
	lui	a1, %hi(.L.str.4)
	addi	s0, a1, %lo(.L.str.4)
	sext.w	a1, a0
	mv	a0, s0
	call	printf
	lui	a0, %hi(.L.str.5)
	addi	a0, a0, %lo(.L.str.5)
	addi	a1, zero, 8
	addi	a2, zero, 8
	addi	a3, zero, 8
	call	printf
	msettypei	a0, e16, int16
	msettypehi	a0, bu
	msettilemi	a1, 8
	msettileni	a2, 8
	msettileki	a3, 8
	lui	a0, %hi(.L.str.6)
	addi	a0, a0, %lo(.L.str.6)
	sext.w	a1, a1
	sext.w	a2, a2
	sext.w	a3, a3
	call	printf
	#APP
	csrr	a0, mtype
	#NO_APP
	sext.w	a1, a0
	mv	a0, s2
	call	printf
	#APP
	csrr	a0, mtilem
	#NO_APP
	sext.w	a1, a0
	mv	a0, s1
	call	printf
	#APP
	csrr	a0, mtilen
	#NO_APP
	sext.w	a1, a0
	mv	a0, s0
	call	printf
	lui	a0, %hi(A)
	addi	a0, a0, %lo(A)
	addi	a1, zero, 16
	mlae16.m	tr0, (a0), a1
	lui	a0, %hi(B)
	addi	a0, a0, %lo(B)
	mlbe16.m	tr1, (a0), a1
	lui	a0, %hi(C)
	addi	a0, a0, %lo(C)
	mlce16.m	acc0, (a0), a1
	msettypei	a0, e16, int16
	msettypehi	a0, bu
	mmau.h.mm	acc0, tr0, tr1
	lui	a0, %hi(O)
	addi	a0, a0, %lo(O)
	msce16.m	acc0, (a0), a1
	lui	a0, %hi(.Lstr)
	addi	a0, a0, %lo(.Lstr)
	call	puts@plt
	mv	s4, zero
	lui	a0, %hi(D)
	addi	s2, a0, %lo(D)
	lui	a0, %hi(.L.str.8)
	addi	s0, a0, %lo(.L.str.8)
	addi	s3, zero, 128
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	add	s1, s4, s2
	lhu	a1, 0(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 2(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 4(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 6(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 8(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 10(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 12(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 14(s1)
	mv	a0, s0
	call	printf
	addi	a0, zero, 10
	call	putchar@plt
	addi	s4, s4, 16
	bne	s4, s3, .LBB0_1
# %bb.2:
	lui	a0, %hi(.Lstr.14)
	addi	a0, a0, %lo(.Lstr.14)
	call	puts@plt
	mv	s4, zero
	lui	a0, %hi(O)
	addi	s2, a0, %lo(O)
	lui	a0, %hi(.L.str.8)
	addi	s0, a0, %lo(.L.str.8)
	addi	s3, zero, 128
.LBB0_3:                                # =>This Inner Loop Header: Depth=1
	add	s1, s4, s2
	lhu	a1, 0(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 2(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 4(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 6(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 8(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 10(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 12(s1)
	mv	a0, s0
	call	printf
	lhu	a1, 14(s1)
	mv	a0, s0
	call	printf
	addi	a0, zero, 10
	call	putchar@plt
	addi	s4, s4, 16
	bne	s4, s3, .LBB0_3
# %bb.4:
	lui	a0, %hi(.L.str.11)
	addi	a0, a0, %lo(.L.str.11)
	lui	a1, %hi(.L.str.12)
	addi	a1, a1, %lo(.L.str.12)
	call	fopen
	beqz	a0, .LBB0_7
# %bb.5:
	mv	s0, a0
	lui	a0, %hi(O)
	addi	a0, a0, %lo(O)
	addi	a1, zero, 2
	addi	a2, zero, 64
	mv	a3, s0
	call	fwrite
	mv	a0, s0
	call	fclose
	mv	a0, zero
.LBB0_6:
	ld	s4, 0(sp)                       # 8-byte Folded Reload
	ld	s3, 8(sp)                       # 8-byte Folded Reload
	ld	s2, 16(sp)                      # 8-byte Folded Reload
	ld	s1, 24(sp)                      # 8-byte Folded Reload
	ld	s0, 32(sp)                      # 8-byte Folded Reload
	ld	ra, 40(sp)                      # 8-byte Folded Reload
	addi	sp, sp, 48
	ret
.LBB0_7:
	lui	a0, %hi(.L.str.13)
	addi	a0, a0, %lo(.L.str.13)
	call	perror
	addi	a0, zero, 1
	j	.LBB0_6
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
                                        # -- End function
	.type	A,@object                       # @A
	.data
	.globl	A
	.p2align	1
A:
	.half	2732                            # 0xaac
	.half	35967                           # 0x8c7f
	.half	43567                           # 0xaa2f
	.half	38852                           # 0x97c4
	.half	42613                           # 0xa675
	.half	46870                           # 0xb716
	.half	52416                           # 0xccc0
	.half	55329                           # 0xd821
	.half	45891                           # 0xb343
	.half	39502                           # 0x9a4e
	.half	21243                           # 0x52fb
	.half	56226                           # 0xdba2
	.half	30403                           # 0x76c3
	.half	35709                           # 0x8b7d
	.half	32103                           # 0x7d67
	.half	55525                           # 0xd8e5
	.half	41993                           # 0xa409
	.half	27764                           # 0x6c74
	.half	57043                           # 0xded3
	.half	40865                           # 0x9fa1
	.half	20757                           # 0x5115
	.half	42329                           # 0xa559
	.half	55026                           # 0xd6f2
	.half	25190                           # 0x6266
	.half	46884                           # 0xb724
	.half	28677                           # 0x7005
	.half	14935                           # 0x3a57
	.half	19499                           # 0x4c2b
	.half	15430                           # 0x3c46
	.half	58443                           # 0xe44b
	.half	48600                           # 0xbdd8
	.half	3716                            # 0xe84
	.half	39512                           # 0x9a58
	.half	63154                           # 0xf6b2
	.half	52620                           # 0xcd8c
	.half	17868                           # 0x45cc
	.half	14650                           # 0x393a
	.half	25129                           # 0x6229
	.half	17089                           # 0x42c1
	.half	31304                           # 0x7a48
	.half	32230                           # 0x7de6
	.half	51886                           # 0xcaae
	.half	18983                           # 0x4a27
	.half	53226                           # 0xcfea
	.half	43095                           # 0xa857
	.half	34661                           # 0x8765
	.half	51374                           # 0xc8ae
	.half	31455                           # 0x7adf
	.half	24152                           # 0x5e58
	.half	37227                           # 0x916b
	.half	35665                           # 0x8b51
	.half	25741                           # 0x648d
	.half	59045                           # 0xe6a5
	.half	60659                           # 0xecf3
	.half	16921                           # 0x4219
	.half	54793                           # 0xd609
	.half	27469                           # 0x6b4d
	.half	4655                            # 0x122f
	.half	38984                           # 0x9848
	.half	22111                           # 0x565f
	.half	6921                            # 0x1b09
	.half	5710                            # 0x164e
	.half	38804                           # 0x9794
	.half	42478                           # 0xa5ee
	.size	A, 128

	.type	B,@object                       # @B
	.globl	B
	.p2align	1
B:
	.half	2163                            # 0x873
	.half	1325                            # 0x52d
	.half	5072                            # 0x13d0
	.half	24133                           # 0x5e45
	.half	37619                           # 0x92f3
	.half	54566                           # 0xd526
	.half	7877                            # 0x1ec5
	.half	62728                           # 0xf508
	.half	18430                           # 0x47fe
	.half	50997                           # 0xc735
	.half	1871                            # 0x74f
	.half	9198                            # 0x23ee
	.half	7599                            # 0x1daf
	.half	57017                           # 0xdeb9
	.half	2496                            # 0x9c0
	.half	57022                           # 0xdebe
	.half	47954                           # 0xbb52
	.half	64134                           # 0xfa86
	.half	24675                           # 0x6063
	.half	31038                           # 0x793e
	.half	42968                           # 0xa7d8
	.half	52373                           # 0xcc95
	.half	31921                           # 0x7cb1
	.half	52488                           # 0xcd08
	.half	33523                           # 0x82f3
	.half	30243                           # 0x7623
	.half	797                             # 0x31d
	.half	34110                           # 0x853e
	.half	49811                           # 0xc293
	.half	51152                           # 0xc7d0
	.half	3219                            # 0xc93
	.half	44491                           # 0xadcb
	.half	15246                           # 0x3b8e
	.half	7751                            # 0x1e47
	.half	24999                           # 0x61a7
	.half	47227                           # 0xb87b
	.half	56608                           # 0xdd20
	.half	41937                           # 0xa3d1
	.half	16321                           # 0x3fc1
	.half	38143                           # 0x94ff
	.half	52489                           # 0xcd09
	.half	9394                            # 0x24b2
	.half	19129                           # 0x4ab9
	.half	35217                           # 0x8991
	.half	53887                           # 0xd27f
	.half	61909                           # 0xf1d5
	.half	41504                           # 0xa220
	.half	49716                           # 0xc234
	.half	55839                           # 0xda1f
	.half	34199                           # 0x8597
	.half	49866                           # 0xc2ca
	.half	6940                            # 0x1b1c
	.half	18676                           # 0x48f4
	.half	27175                           # 0x6a27
	.half	57495                           # 0xe097
	.half	31037                           # 0x793d
	.half	60067                           # 0xeaa3
	.half	17337                           # 0x43b9
	.half	31230                           # 0x79fe
	.half	12211                           # 0x2fb3
	.half	11723                           # 0x2dcb
	.half	50740                           # 0xc634
	.half	43890                           # 0xab72
	.half	48294                           # 0xbca6
	.size	B, 128

	.type	C,@object                       # @C
	.globl	C
	.p2align	1
C:
	.half	17591                           # 0x44b7
	.half	29894                           # 0x74c6
	.half	22556                           # 0x581c
	.half	14191                           # 0x376f
	.half	25378                           # 0x6322
	.half	4484                            # 0x1184
	.half	10368                           # 0x2880
	.half	8861                            # 0x229d
	.half	26752                           # 0x6880
	.half	1231                            # 0x4cf
	.half	26532                           # 0x67a4
	.half	21242                           # 0x52fa
	.half	23605                           # 0x5c35
	.half	7709                            # 0x1e1d
	.half	6021                            # 0x1785
	.half	9809                            # 0x2651
	.half	20006                           # 0x4e26
	.half	7346                            # 0x1cb2
	.half	3560                            # 0xde8
	.half	14570                           # 0x38ea
	.half	25332                           # 0x62f4
	.half	7663                            # 0x1def
	.half	28945                           # 0x7111
	.half	25328                           # 0x62f0
	.half	31055                           # 0x794f
	.half	29081                           # 0x7199
	.half	12676                           # 0x3184
	.half	26384                           # 0x6710
	.half	18025                           # 0x4669
	.half	11915                           # 0x2e8b
	.half	27690                           # 0x6c2a
	.half	29487                           # 0x732f
	.half	13754                           # 0x35ba
	.half	23560                           # 0x5c08
	.half	14879                           # 0x3a1f
	.half	7409                            # 0x1cf1
	.half	21368                           # 0x5378
	.half	28641                           # 0x6fe1
	.half	20737                           # 0x5101
	.half	26368                           # 0x6700
	.half	30017                           # 0x7541
	.half	12951                           # 0x3297
	.half	28647                           # 0x6fe7
	.half	6506                            # 0x196a
	.half	28585                           # 0x6fa9
	.half	3946                            # 0xf6a
	.half	26681                           # 0x6839
	.half	30789                           # 0x7845
	.half	14627                           # 0x3923
	.half	10929                           # 0x2ab1
	.half	12134                           # 0x2f66
	.half	10036                           # 0x2734
	.half	27767                           # 0x6c77
	.half	11182                           # 0x2bae
	.half	15115                           # 0x3b0b
	.half	11200                           # 0x2bc0
	.half	8622                            # 0x21ae
	.half	13787                           # 0x35db
	.half	23634                           # 0x5c52
	.half	23471                           # 0x5baf
	.half	20571                           # 0x505b
	.half	8449                            # 0x2101
	.half	30592                           # 0x7780
	.half	16428                           # 0x402c
	.size	C, 128

	.type	D,@object                       # @D
	.globl	D
	.p2align	1
D:
	.half	59910                           # 0xea06
	.half	12427                           # 0x308b
	.half	14565                           # 0x38e5
	.half	40311                           # 0x9d77
	.half	65056                           # 0xfe20
	.half	10037                           # 0x2735
	.half	18446                           # 0x480e
	.half	30540                           # 0x774c
	.half	37376                           # 0x9200
	.half	11649                           # 0x2d81
	.half	33431                           # 0x8297
	.half	55668                           # 0xd974
	.half	22548                           # 0x5814
	.half	6285                            # 0x188d
	.half	47747                           # 0xba83
	.half	48197                           # 0xbc45
	.half	47545                           # 0xb9b9
	.half	14313                           # 0x37e9
	.half	52998                           # 0xcf06
	.half	21137                           # 0x5291
	.half	919                             # 0x397
	.half	23424                           # 0x5b80
	.half	25459                           # 0x6373
	.half	50528                           # 0xc560
	.half	16331                           # 0x3fcb
	.half	38333                           # 0x95bd
	.half	30744                           # 0x7818
	.half	32239                           # 0x7def
	.half	15770                           # 0x3d9a
	.half	61272                           # 0xef58
	.half	17868                           # 0x45cc
	.half	29148                           # 0x71dc
	.half	57646                           # 0xe12e
	.half	4861                            # 0x12fd
	.half	54270                           # 0xd3fe
	.half	42752                           # 0xa700
	.half	21909                           # 0x5595
	.half	14157                           # 0x374d
	.half	44522                           # 0xadea
	.half	31175                           # 0x79c7
	.half	38577                           # 0x96b1
	.half	24727                           # 0x6097
	.half	2652                            # 0xa5c
	.half	21569                           # 0x5441
	.half	7131                            # 0x1bdb
	.half	10237                           # 0x27fd
	.half	22323                           # 0x5733
	.half	59548                           # 0xe89c
	.half	54641                           # 0xd571
	.half	49142                           # 0xbff6
	.half	41757                           # 0xa31d
	.half	12315                           # 0x301b
	.half	10647                           # 0x2997
	.half	21837                           # 0x554d
	.half	61825                           # 0xf181
	.half	43715                           # 0xaac3
	.half	20566                           # 0x5056
	.half	1489                            # 0x5d1
	.half	35367                           # 0x8a27
	.half	52911                           # 0xceaf
	.half	38524                           # 0x967c
	.half	65441                           # 0xffa1
	.half	5415                            # 0x1527
	.half	57202                           # 0xdf72
	.size	D, 128

	.type	O,@object                       # @O
	.bss
	.globl	O
	.p2align	1
O:
	.zero	128
	.size	O, 128

	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"mlenb(0xc44): %d, "
	.size	.L.str, 19

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"mrlenb(0xc45): %d \r\n"
	.size	.L.str.1, 21

	.type	.L.str.2,@object                # @.str.2
.L.str.2:
	.asciz	"mtilem(0xc40): %d, "
	.size	.L.str.2, 20

	.type	.L.str.3,@object                # @.str.3
.L.str.3:
	.asciz	"mtilen(0xc41): %d \r\n"
	.size	.L.str.3, 21

	.type	.L.str.4,@object                # @.str.4
.L.str.4:
	.asciz	"mtilek(0xc42): %d \r\n"
	.size	.L.str.4, 21

	.type	.L.str.5,@object                # @.str.5
.L.str.5:
	.asciz	"m = %d, k = %d, n = %d\n"
	.size	.L.str.5, 24

	.type	.L.str.6,@object                # @.str.6
.L.str.6:
	.asciz	"tile_m %d, tile_n %d, tile_k %d\n"
	.size	.L.str.6, 33

	.type	.L.str.8,@object                # @.str.8
.L.str.8:
	.asciz	"%04hx "
	.size	.L.str.8, 7

	.type	.L.str.11,@object               # @.str.11
.L.str.11:
	.asciz	"./tests/test-progs/MMI/outdata.bin"
	.size	.L.str.11, 35

	.type	.L.str.12,@object               # @.str.12
.L.str.12:
	.asciz	"wb"
	.size	.L.str.12, 3

	.type	.L.str.13,@object               # @.str.13
.L.str.13:
	.asciz	"fopen"
	.size	.L.str.13, 6

	.type	.Lstr,@object                   # @str
.Lstr:
	.asciz	"[ --- Golden Data --- ] "
	.size	.Lstr, 25

	.type	.Lstr.14,@object                # @str.14
.Lstr.14:
	.asciz	"[ --- Output Data --- ] "
	.size	.Lstr.14, 25

	.ident	"clang version 14.0.0 (https://github.com/riscv-stc/llvm-project.git 70d1e5ec4b9a3dcaf4e8731c83333c5f98618e27)"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym A
	.addrsig_sym B
	.addrsig_sym C
	.addrsig_sym O
