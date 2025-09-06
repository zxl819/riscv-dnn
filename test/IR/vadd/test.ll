; 最小示例：调用 llvm.riscv.vadd.nxv4i32.nxv4i32（请先用 grep 确认你的 LLVM 中的名字）
; 声明（按你 LLVM 版本实际名字/签名调整）
declare <vscale x 4 x i32> @llvm.riscv.vadd.nxv4i32.nxv4i32(<vscale x 4 x i32>, <vscale x 4 x i32>, <vscale x 4 x i32>, i64)

define <vscale x 4 x i32> @foo(<vscale x 4 x i32> %a, <vscale x 4 x i32> %b, i64 %vl) {
entry:
  ; 第一个参数可能是 'poison'，用 undef 填充
  %r = call <vscale x 4 x i32> @llvm.riscv.vadd.nxv4i32.nxv4i32(undef, %a, %b, %vl)
  ret <vscale x 4 x i32> %r
}