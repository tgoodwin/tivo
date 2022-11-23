; ModuleID = 'randNo.c'
source_filename = "randNo.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx12.0.0"

@MyRand.rand = internal global i32 44257, align 4
@.str = private unnamed_addr constant [4 x i8] c"%u\09\00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @MyRand(i32 %0, i32 %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 %0, i32* %4, align 4
  store i32 %1, i32* %5, align 4
  %6 = load i32, i32* %4, align 4
  %7 = load i32, i32* %5, align 4
  %8 = icmp eq i32 %6, %7
  br i1 %8, label %9, label %11

9:                                                ; preds = %2
  %10 = load i32, i32* %4, align 4
  store i32 %10, i32* %3, align 4
  br label %29

11:                                               ; preds = %2
  %12 = load i32, i32* @MyRand.rand, align 4
  %13 = add i32 %12, 941
  store i32 %13, i32* @MyRand.rand, align 4
  %14 = load i32, i32* %5, align 4
  %15 = load i32, i32* @MyRand.rand, align 4
  %16 = urem i32 %15, %14
  store i32 %16, i32* @MyRand.rand, align 4
  br label %17

17:                                               ; preds = %21, %11
  %18 = load i32, i32* @MyRand.rand, align 4
  %19 = load i32, i32* %4, align 4
  %20 = icmp ult i32 %18, %19
  br i1 %20, label %21, label %27

21:                                               ; preds = %17
  %22 = load i32, i32* @MyRand.rand, align 4
  %23 = load i32, i32* %5, align 4
  %24 = add i32 %22, %23
  %25 = load i32, i32* %4, align 4
  %26 = sub i32 %24, %25
  store i32 %26, i32* @MyRand.rand, align 4
  br label %17, !llvm.loop !10

27:                                               ; preds = %17
  %28 = load i32, i32* @MyRand.rand, align 4
  store i32 %28, i32* %3, align 4
  br label %29

29:                                               ; preds = %27, %9
  %30 = load i32, i32* %3, align 4
  ret i32 %30
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 0, i32* %2, align 4
  br label %3

3:                                                ; preds = %9, %0
  %4 = load i32, i32* %2, align 4
  %5 = icmp slt i32 %4, 255
  br i1 %5, label %6, label %12

6:                                                ; preds = %3
  %7 = call i32 @MyRand(i32 10, i32 20)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %7)
  br label %9

9:                                                ; preds = %6
  %10 = load i32, i32* %2, align 4
  %11 = add nsw i32 %10, 1
  store i32 %11, i32* %2, align 4
  br label %3, !llvm.loop !12

12:                                               ; preds = %3
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { noinline nounwind optnone ssp uwtable "frame-pointer"="non-leaf" "min-legal-vector-width"="0" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.5a,+zcm,+zcz" }
attributes #1 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+crc,+crypto,+dotprod,+fp-armv8,+fp16fml,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+sha3,+sm4,+v8.5a,+zcm,+zcz" }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5, !6, !7, !8}
!llvm.ident = !{!9}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 12, i32 3]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 1, !"branch-target-enforcement", i32 0}
!3 = !{i32 1, !"sign-return-address", i32 0}
!4 = !{i32 1, !"sign-return-address-all", i32 0}
!5 = !{i32 1, !"sign-return-address-with-bkey", i32 0}
!6 = !{i32 7, !"PIC Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 1}
!8 = !{i32 7, !"frame-pointer", i32 1}
!9 = !{!"Apple clang version 13.1.6 (clang-1316.0.21.2.5)"}
!10 = distinct !{!10, !11}
!11 = !{!"llvm.loop.mustprogress"}
!12 = distinct !{!12, !11}
