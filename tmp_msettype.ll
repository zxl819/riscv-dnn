; ModuleID = 'tmp_msettype.c'
source_filename = "tmp_msettype.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n64-S128"
target triple = "riscv64-unknown-unknown-elf"

; Function Attrs: nounwind
define dso_local i64 @demo_settype_low(i64 %0) local_unnamed_addr #0 {
  %2 = tail call i64 @llvm.riscv.msettypei.i64(i64 %0) #1
  ret i64 %2
}

; Function Attrs: nounwind
define dso_local i64 @demo_settype_hi(i64 %0) local_unnamed_addr #0 {
  %2 = icmp eq i64 %0, 0
  br i1 %2, label %5, label %3

3:                                                ; preds = %1
  %4 = tail call i64 @llvm.riscv.msettypehi.i64(i64 %0) #1
  br label %5

5:                                                ; preds = %1, %3
  %6 = phi i64 [ %4, %3 ], [ 0, %1 ]
  ret i64 %6
}

; Function Attrs: nounwind
define dso_local signext i32 @main() local_unnamed_addr #0 {
  %1 = tail call i64 @llvm.riscv.msettypei.i64(i64 1) #1
  %2 = tail call i64 @llvm.riscv.msettypehi.i64(i64 16) #1
  %3 = add i64 %2, %1
  %4 = trunc i64 %3 to i32
  ret i32 %4
}

; Function Attrs: nounwind
declare i64 @llvm.riscv.msettypei.i64(i64) #1

; Function Attrs: nounwind
declare i64 @llvm.riscv.msettypehi.i64(i64) #1

attributes #0 = { nounwind "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-features"="+64bit,+a,+c,+d,+experimental-matrix,+experimental-v,+experimental-zfh,+experimental-zvlsseg,+f,+m,+relax,-save-restore" }
attributes #1 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"target-abi", !"lp64d"}
!2 = !{i32 1, !"SmallDataLimit", i32 8}
!3 = !{!"clang version 14.0.0 (git@github.com:riscv-stc/llvm-project.git 70d1e5ec4b9a3dcaf4e8731c83333c5f98618e27)"}
