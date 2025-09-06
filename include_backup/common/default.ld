/* default.ld : minimal bare-metal for Spike + HTIF + TLS */

OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY {
  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 128M
}

/* 供启动代码使用的栈顶（可以改成固定地址或减去保留区） */
PROVIDE(_stack_top = ORIGIN(RAM) + LENGTH(RAM));

SECTIONS {
  /* 启动与常规代码 */
  .text.init : {
    KEEP(*(.text.init))
  } > RAM

  .text : ALIGN(8) {
    *(.text .text.* .gnu.linkonce.t.*)
  } > RAM

  /* 只读数据 */
  .rodata : ALIGN(8) {
    *(.rodata .rodata.* .gnu.linkonce.r.*)
  } > RAM

  /* 可写数据 */
  .data : ALIGN(8) {
    _data = .;
    __global_pointer$ = . + 0x800;      /* 可选：供 gp 小数据寻址 */
    *(.data .data.* .gnu.linkonce.d.*)
    _edata = .;
  } > RAM

  /* 线程局部存储：初始化数据段 */
  .tdata : ALIGN(8) {
    _tdata_begin = .;
    *(.tdata .tdata.* .gnu.linkonce.td.*)
    _tdata_end = .;
  } > RAM

  /* 线程局部存储：未初始化（不占用镜像） */
  .tbss (NOLOAD) : ALIGN(8) {
    _tbss_begin = .;
    *(.tbss .tbss.* .gnu.linkonce.tb.*)
    *(.tcommon)
    _tbss_end = .;
  } > RAM

  /* BSS */
  .bss (NOLOAD) : ALIGN(8) {
    _bss = .;
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    _ebss = .;
  } > RAM

  /* Spike 裸机半主机接口（HTIF） */
  .tohost : ALIGN(8) {
    PROVIDE(tohost = .);
    . += 8;      /* 8 字节占位 */
  } > RAM

  .fromhost : ALIGN(8) {
    PROVIDE(fromhost = .);
    . += 8;      /* 8 字节占位 */
  } > RAM

  /* small read-only data & 工具链自定义只读小段 */
  .srodata : ALIGN(16) {
    *(.srodata .srodata.* .gnu.linkonce.sr.*)
    *(.sdata2 .sdata2.*)                  /* 一些工具链把只读小数据放 sdata2 */
    *(.scdata .scdata.* .scdata.params)   /* ← 你的 .scdata.params 在这里接住 */
    *(.rodata1)                           /* 偶尔见到 */
  } > RAM

  /* small writable data */
  .sdata : ALIGN(16) {
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  } > RAM

  /* small bss */
  .sbss (NOLOAD) : ALIGN(16) {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  } > RAM

  /* C/C++ 预初始化（有些库会用到，顺序：preinit → init） */
  .preinit_array : ALIGN(8) {
    PROVIDE_HIDDEN(__preinit_array_start = .);
    KEEP(*(.preinit_array .preinit_array.*))
    PROVIDE_HIDDEN(__preinit_array_end = .);
  } > RAM

  /* C++ 构造/析构 & 初始化数组 */
  .init_array : ALIGN(8) {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(.init_array .init_array.*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } > RAM

  .fini_array : ALIGN(8) {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(.fini_array .fini_array.*))
    PROVIDE_HIDDEN (__fini_array_end = .);
  } > RAM

  /* 旧式构造/析构支持（以防万一）*/
  .ctors : ALIGN(8) {
    KEEP (*(.ctors .ctors.*))
  } > RAM
  .dtors : ALIGN(8) {
    KEEP (*(.dtors .dtors.*))
  } > RAM

  /* 异常处理/展开表（libunwind/c++ 可能需要） */
  .eh_frame_hdr : ALIGN(8) {
    *(.eh_frame_hdr)
  } > RAM
  .eh_frame : ALIGN(8) {
    KEEP(*(.eh_frame))
  } > RAM
  .gcc_except_table : ALIGN(8) {
    *(.gcc_except_table .gcc_except_table.*)
  } > RAM

  /* 读写只读数据（C++ vtable 等）*/
  .data.rel.ro : ALIGN(8) {
    *(.data.rel.ro .data.rel.ro.*)
  } > RAM

  /* —— 你已有的 BSS、HTIF、_end 等放在它们后面 —— */

  /* 丢弃无关段，避免把一些标记段误放进 RAM */
  /DISCARD/ : {
    *(.note.GNU-stack)
    *(.gnu_debuglink)
    *(.gnu.lto_*)
    *(.comment)
    *(.riscv.attributes)
  }


  /* 结束符号/堆起点 */
  _end = .;
  PROVIDE(end = .);
  PROVIDE(__heap_start = .);
}

