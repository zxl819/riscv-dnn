/* Compact RISC-V test linker script
 * 目标：保持与原 test.ld 相似的运行地址窗口，但避免巨大 ALIGN 造成数百 MB 文件。
 * 方法：
 *  - 代码/常量紧凑放在 ROM 区域 (0x80000000 起)。
 *  - 数据/小型参数放 RAM (0x80400000 起)。
 *  - signature 和 perf 数据放独立 NOLOAD 段 (SIG / PERF)；不写入 ELF 文件，仿真器可按符号定位。
 *  - 若需要真正落盘，可去掉 NOLOAD。
 */

OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
  ROM (rx)  : ORIGIN = 0x80000000, LENGTH = 4M
  RAM (rwx) : ORIGIN = 0x80400000, LENGTH = 8M
  SIG (rw)  : ORIGIN = 0x90000000, LENGTH = 1M
  PERF (rw) : ORIGIN = 0x90100000, LENGTH = 1M
}

SECTIONS
{
  .text.init : { *(.text.init) } > ROM
  .text      : { *(.text) } > ROM
  .rodata    : { *(.rodata .rodata.*) } > ROM
  .rodata.str1.1 : { *(.rodata.str1.1) } > ROM

  .data : {
    *(.data .data.*)
    *(.sdata .sdata.*)
  } > RAM

  /* TLS data sections (to satisfy init_tls in syscalls.c) */
  .tdata : {
    _tdata_begin = .;
    *(.tdata .tdata.*)
    _tdata_end = .;
  } > RAM

  .tbss (NOLOAD) : {
    *(.tbss .tbss.*)
    _tbss_end = .;
  } > RAM

  .bss (NOLOAD) : {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.bss .bss.*)
    *(COMMON)
  } > RAM

  .params : { *(.scdata.params) } > RAM

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
}
