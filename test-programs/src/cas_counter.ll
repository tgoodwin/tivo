; ModuleID = 'cas_counter.c'
source_filename = "cas_counter.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx12.0.0"

%struct._opaque_pthread_t = type { i64, %struct.__darwin_pthread_handler_rec*, [8176 x i8] }
%struct.__darwin_pthread_handler_rec = type { void (i8*)*, i8*, %struct.__darwin_pthread_handler_rec* }
%struct._opaque_pthread_attr_t = type { i64, [56 x i8] }

@counter = global i32 0, align 4
@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: noinline nounwind optnone ssp uwtable
define void @counter_inc() #0 {
  %1 = alloca i32, align 4
  br label %2

2:                                                ; preds = %4, %0
  %3 = load volatile i32, i32* @counter, align 4
  store i32 %3, i32* %1, align 4
  br label %4

4:                                                ; preds = %2
  %5 = load i32, i32* %1, align 4
  %6 = load i32, i32* %1, align 4
  %7 = add nsw i32 %6, 1
  %8 = cmpxchg i32* @counter, i32 %5, i32 %7 seq_cst seq_cst, align 4
  %9 = extractvalue { i32, i1 } %8, 1
  %10 = xor i1 %9, true
  br i1 %10, label %2, label %11, !llvm.loop !10

11:                                               ; preds = %4
  ret void
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i8* @thread_main(i8* %0) #0 {
  %2 = alloca i8*, align 8
  %3 = alloca i32, align 4
  store i8* %0, i8** %2, align 8
  store i32 0, i32* %3, align 4
  br label %4

4:                                                ; preds = %13, %1
  %5 = load i32, i32* %3, align 4
  %6 = icmp slt i32 %5, 100000000
  br i1 %6, label %7, label %16

7:                                                ; preds = %4
  %8 = load i32, i32* %3, align 4
  %9 = srem i32 %8, 2
  %10 = icmp eq i32 %9, 0
  br i1 %10, label %11, label %12

11:                                               ; preds = %7
  call void @counter_inc()
  br label %12

12:                                               ; preds = %11, %7
  br label %13

13:                                               ; preds = %12
  %14 = load i32, i32* %3, align 4
  %15 = add nsw i32 %14, 1
  store i32 %15, i32* %3, align 4
  br label %4, !llvm.loop !12

16:                                               ; preds = %4
  ret i8* null
}

; Function Attrs: noinline nounwind optnone ssp uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca [5 x %struct._opaque_pthread_t*], align 8
  %3 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  store i32 0, i32* %3, align 4
  br label %4

4:                                                ; preds = %12, %0
  %5 = load i32, i32* %3, align 4
  %6 = icmp slt i32 %5, 5
  br i1 %6, label %7, label %15

7:                                                ; preds = %4
  %8 = load i32, i32* %3, align 4
  %9 = sext i32 %8 to i64
  %10 = getelementptr inbounds [5 x %struct._opaque_pthread_t*], [5 x %struct._opaque_pthread_t*]* %2, i64 0, i64 %9
  %11 = call i32 @pthread_create(%struct._opaque_pthread_t** %10, %struct._opaque_pthread_attr_t* null, i8* (i8*)* @thread_main, i8* null)
  br label %12

12:                                               ; preds = %7
  %13 = load i32, i32* %3, align 4
  %14 = add nsw i32 %13, 1
  store i32 %14, i32* %3, align 4
  br label %4, !llvm.loop !13

15:                                               ; preds = %4
  store i32 0, i32* %3, align 4
  br label %16

16:                                               ; preds = %25, %15
  %17 = load i32, i32* %3, align 4
  %18 = icmp slt i32 %17, 5
  br i1 %18, label %19, label %28

19:                                               ; preds = %16
  %20 = load i32, i32* %3, align 4
  %21 = sext i32 %20 to i64
  %22 = getelementptr inbounds [5 x %struct._opaque_pthread_t*], [5 x %struct._opaque_pthread_t*]* %2, i64 0, i64 %21
  %23 = load %struct._opaque_pthread_t*, %struct._opaque_pthread_t** %22, align 8
  %24 = call i32 @"\01_pthread_join"(%struct._opaque_pthread_t* %23, i8** null)
  br label %25

25:                                               ; preds = %19
  %26 = load i32, i32* %3, align 4
  %27 = add nsw i32 %26, 1
  store i32 %27, i32* %3, align 4
  br label %16, !llvm.loop !14

28:                                               ; preds = %16
  %29 = load volatile i32, i32* @counter, align 4
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %29)
  ret i32 0
}

declare i32 @pthread_create(%struct._opaque_pthread_t**, %struct._opaque_pthread_attr_t*, i8* (i8*)*, i8*) #1

declare i32 @"\01_pthread_join"(%struct._opaque_pthread_t*, i8**) #1

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
!13 = distinct !{!13, !11}
!14 = distinct !{!14, !11}
