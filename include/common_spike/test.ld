/*======================================================================*/
/* Proxy kernel linker script                                           */
/*======================================================================*/
/* This is the linker script used when building the proxy kernel. */

/*----------------------------------------------------------------------*/
/* Setup                                                                */
/*----------------------------------------------------------------------*/

/* The OUTPUT_ARCH command specifies the machine architecture where the
   argument is one of the names used in the BFD library. More
   specifically one of the entires in bfd/cpu-mips.c */

OUTPUT_ARCH( "riscv" )
ENTRY(_start)

/*----------------------------------------------------------------------*/
/* Sections                                                             */
/*----------------------------------------------------------------------*/

SECTIONS
{

  /* text: test code section */
  . = 0x80000000;
  .text.init : { *(.text.init) }

  . = ALIGN(0x1000);
  .tohost : { *(.tohost) }

  . = ALIGN(0x1000);
  .text : { *(.text) }

  /* data segment */
  .data : { 
    *(.data) 
    *(.scdata.params)
    . = ALIGN(64);
    begin_signature = .;
    *(.scdata.output)
    . = ALIGN(64);
    begin_perf_data = .;
    *(.pfdata.output)
    end_perf_data = .;
    end_signature = .;
    }
  /* 矩阵区：紧随其后（不再大跳 0x80040000），仍 64B 对齐 */
  . = ALIGN(64);
  _matA_start = .;
  .matA : { *(.matA) }
  _matA_end = .;
  ASSERT(SIZEOF(.matA) <= 0x200, "matA too big (>0x200)")

  . = ALIGN(64);
  _matB_start = .;
  .matB : { *(.matB) }
  _matB_end = .;
  ASSERT(SIZEOF(.matB) <= 0x200, "matB too big (>0x200)")

  . = ALIGN(64);
  _matC_start = .;
  .matC (NOLOAD) : { *(.matC) }
  _matC_end = .;
  ASSERT(SIZEOF(.matC) <= 0x800, "matC too big (>0x800)")


  /*.sdata : {
    __global_pointer$ = . + 0x8000;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  }
  */

  /* bss segment */
  .sbss : {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  }
  .bss : { *(.bss) }

  /* thread-local data segment */
  .tdata :
  {
    _tdata_begin = .;
    *(.tdata)
    _tdata_end = .;
  }
  .tbss :
  {
    *(.tbss)
    _tbss_end = .;
  }

  

  /* End of uninitalized data segement */
  _end = .;
}

