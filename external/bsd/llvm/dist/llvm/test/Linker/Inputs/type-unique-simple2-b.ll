; ModuleID = 'bar.cpp'

%struct.Base = type { i32, %struct.Base* }

; Function Attrs: nounwind ssp uwtable
define void @_Z1gi(i32 %a) #0 {
entry:
  %a.addr = alloca i32, align 4
  %t = alloca %struct.Base, align 8
  store i32 %a, i32* %a.addr, align 4
  call void @llvm.dbg.declare(metadata !{i32* %a.addr}, metadata !20), !dbg !21
  call void @llvm.dbg.declare(metadata !{%struct.Base* %t}, metadata !22), !dbg !23
  ret void, !dbg !24
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata) #1

; Function Attrs: ssp uwtable
define i32 @main() #2 {
entry:
  %retval = alloca i32, align 4
  store i32 0, i32* %retval
  call void @_Z1fi(i32 0), !dbg !25
  call void @_Z1gi(i32 1), !dbg !26
  ret i32 0, !dbg !27
}

declare void @_Z1fi(i32) #3

attributes #0 = { nounwind ssp uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }
attributes #2 = { ssp uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!19}

!0 = metadata !{i32 786449, metadata !1, i32 4, metadata !"clang version 3.4 (http://llvm.org/git/clang.git 8a3f9e46cb988d2c664395b21910091e3730ae82) (http://llvm.org/git/llvm.git 4699e9549358bc77824a59114548eecc3f7c523c)", i1 false, metadata !"", i32 0, metadata !2, metadata !3, metadata !11, metadata !2, metadata !2, metadata !""} ; [ DW_TAG_compile_unit ] [bar.cpp] [DW_LANG_C_plus_plus]
!1 = metadata !{metadata !"bar.cpp", metadata !"."}
!2 = metadata !{i32 0}
!3 = metadata !{metadata !4}
!4 = metadata !{i32 786451, metadata !5, null, metadata !"Base", i32 1, i64 128, i64 64, i32 0, i32 0, null, metadata !6, i32 0, null, null, metadata !"_ZTS4Base"} ; [ DW_TAG_structure_type ] [Base] [line 1, size 128, align 64, offset 0] [def] [from ]
!5 = metadata !{metadata !"./a.hpp", metadata !"."}
!6 = metadata !{metadata !7, metadata !9}
!7 = metadata !{i32 786445, metadata !5, metadata !"_ZTS4Base", metadata !"a", i32 2, i64 32, i64 32, i64 0, i32 0, metadata !8} ; [ DW_TAG_member ] [a] [line 2, size 32, align 32, offset 0] [from int]
!8 = metadata !{i32 786468, null, null, metadata !"int", i32 0, i64 32, i64 32, i64 0, i32 0, i32 5} ; [ DW_TAG_base_type ] [int] [line 0, size 32, align 32, offset 0, enc DW_ATE_signed]
!9 = metadata !{i32 786445, metadata !5, metadata !"_ZTS4Base", metadata !"b", i32 3, i64 64, i64 64, i64 64, i32 0, metadata !10} ; [ DW_TAG_member ] [b] [line 3, size 64, align 64, offset 64] [from ]
!10 = metadata !{i32 786447, null, null, metadata !"", i32 0, i64 64, i64 64, i64 0, i32 0, metadata !"_ZTS4Base"}
!11 = metadata !{metadata !12, metadata !16}
!12 = metadata !{i32 786478, metadata !1, metadata !13, metadata !"g", metadata !"g", metadata !"_Z1gi", i32 4, metadata !14, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, void (i32)* @_Z1gi, null, null, metadata !2, i32 4} ; [ DW_TAG_subprogram ] [line 4] [def] [g]
!13 = metadata !{i32 786473, metadata !1}         ; [ DW_TAG_file_type ] [bar.cpp]
!14 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !15, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!15 = metadata !{null, metadata !8}
!16 = metadata !{i32 786478, metadata !1, metadata !13, metadata !"main", metadata !"main", metadata !"", i32 7, metadata !17, i1 false, i1 true, i32 0, i32 0, null, i32 256, i1 false, i32 ()* @main, null, null, metadata !2, i32 7} ; [ DW_TAG_subprogram ] [line 7] [def] [main]
!17 = metadata !{i32 786453, i32 0, null, metadata !"", i32 0, i64 0, i64 0, i64 0, i32 0, null, metadata !18, i32 0, null, null, null} ; [ DW_TAG_subroutine_type ] [line 0, size 0, align 0, offset 0] [from ]
!18 = metadata !{metadata !8}
!19 = metadata !{i32 2, metadata !"Dwarf Version", i32 2}
!20 = metadata !{i32 786689, metadata !12, metadata !"a", metadata !13, i32 16777220, metadata !8, i32 0, i32 0} ; [ DW_TAG_arg_variable ] [a] [line 4]
!21 = metadata !{i32 4, i32 0, metadata !12, null}
!22 = metadata !{i32 786688, metadata !12, metadata !"t", metadata !13, i32 5, metadata !4, i32 0, i32 0} ; [ DW_TAG_auto_variable ] [t] [line 5]
!23 = metadata !{i32 5, i32 0, metadata !12, null}
!24 = metadata !{i32 6, i32 0, metadata !12, null}
!25 = metadata !{i32 8, i32 0, metadata !16, null} ; [ DW_TAG_imported_declaration ]
!26 = metadata !{i32 9, i32 0, metadata !16, null}
!27 = metadata !{i32 10, i32 0, metadata !16, null}
