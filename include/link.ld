/* Copyright 2018 Google LLC
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http:*www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

OUTPUT_ARCH( "riscv" )
ENTRY(_start)

SECTIONS
{
  . = 0x80000000;
  _start_text = .;
  .text.init : { *(.text.init) }
  . = ALIGN(0x1000);
  .tohost : { *(.tohost) }
  . = ALIGN(0x1000);
  .uvmif : { *(.uvmif) }
  . = ALIGN(0x1000);
  .text : { *(.text) }
  . = ALIGN(0x1000);
  .text.startup : { *(.text.startup) }
  . = ALIGN(0x1000); 
  _end_text = .;
  . = ALIGN(0x1000);
  .rodata : { *(.rodata*)}
  . = ALIGN(0x8);
  . = ALIGN(0x1000);
  .page_table : { *(.page_table) }
  .user_stack : { *(.user_stack) }
  .kernel_data : { *(.kernel_data) }
  .kernel_stack : { *(.kernel_stack) }
  .data : { *(.data) }
  .sdata : {
    __global_pointer$ = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
    
  }
/* mat data address 
. = 0x0; _matA_start = .; .matA : { *(.matA) } _matA_end = .;
. = 0x1000; _matB_start = .; .matB : { *(.matB) } _matB_end = .;
. = 0x2000; _matC_start = .; .matC : { *(.matC) } _matC_end = .;
outrange
*/
. = 0x80040000; _matA_start = .; .matA : { *(.matA) } _matA_end = .;
. = 0x80050000; _matB_start = .; .matB : { *(.matB) } _matB_end = .;
. = 0x80060000; _matC_start = .; .matC : { *(.matC) } _matC_end = .;

/*. = 0x80040000;
. = ALIGN(64); _matA_start = .; .matA : { *(.matA) } _matA_end = .;
. = ALIGN(64); _matB_start = .; .matB : { *(.matB) } _matB_end = .;
. = ALIGN(64); _matC_start = .; .matC : { *(.matC) } _matC_end = .;
*/
  /* bss segment */
/*  .sbss : {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  }*/
  .bss : { *(.bss) }
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

  _end = .;
}
