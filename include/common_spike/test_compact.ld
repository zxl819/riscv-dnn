OUTPUT_ARCH("riscv")
ENTRY(_start)

/*
可选：
方案A(默认): ROM=1MB  0x8000_0000 - 0x800F_FFFF
方案B(更小): 改 LENGTH = 512K (0x80000) 若最终大小 < 512KB
*/

MEMORY
{
  ROM  (rwx) : ORIGIN = 0x80000000, LENGTH = 4M      /* 若需要更小可改 512K */
  SIG  (rw)  : ORIGIN = 0x80100000, LENGTH = 1M
  PERF (rw)  : ORIGIN = 0x80140000, LENGTH = 1M
}

SECTIONS
{
  . = ORIGIN(ROM);

  /* 紧凑排布，保持所有常用段落在一个连续窗口内，利于 -mno-relax */
  .text.init : { *(.text.init) } > ROM
  .text      : { *(.text .text.*) } > ROM
  .rodata    : { *(.rodata .rodata.* .srodata .srodata.*) } > ROM
  .rodata.str1.1 : { *(.rodata.str1.1) } > ROM

  . = ALIGN(64);
  .data : { *(.data .data.*) } > ROM

  . = ALIGN(16);
  .sdata : {
    __global_pointer$ = . + 0x800;
    *(.sdata .sdata.*)
  } > ROM

  /* 矩阵段紧随放置，仍保持 64B 对齐 */
  . = ALIGN(64);
  _matA_start = .;
  .matA : { *(.matA) } > ROM
  _matA_end = .;  ASSERT(SIZEOF(.matA) <= 0x200, "matA too big (>0x200)")

  . = ALIGN(64);
  _matB_start = .;
  .matB : { *(.matB) } > ROM
  _matB_end = .;  ASSERT(SIZEOF(.matB) <= 0x200, "matB too big (>0x200)")

  . = ALIGN(64);
  _matC_start = .;
  .matC (NOLOAD) : { *(.matC) } > ROM
  _matC_end = .;  ASSERT(SIZEOF(.matC) <= 0x800, "matC too big (>0x800)")

  . = ALIGN(16);
  .bss : {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(COMMON)
  } > ROM

  .tdata : { _tdata_begin = .; *(.tdata .tdata.*) _tdata_end = .; } > ROM
  .tbss  (NOLOAD) : { *(.tbss .tbss.*) _tbss_end = .; } > ROM

  .signature (NOLOAD) : {
    begin_signature = .;
    *(.scdata.output)
    end_signature = .;
  } > SIG

  .perfdata (NOLOAD) : {
    begin_perf_data = .;
    *(.pfdata.output)
    end_perf_data = .;
  } > PERF

  _end = .;

  /* 尺寸检查：总大小不要超过 ROM 长度 */
  ASSERT(_end - ORIGIN(ROM) <= LENGTH(ROM), "ROM overflow: enlarge LENGTH")
}