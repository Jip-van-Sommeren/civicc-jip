_fun0_foo:
    iload_0
    fload_1
    f2i
    iadd
    iload_2
    iadd
    ireturn

_fun1_fun:
    esr 7
    iloadc_1
    istore 0
    iloadc 0
    istore 1
    iloadc 1
    istore 2
    iloadc 2
    istore 3
    floadc_1
    fstore 4
    floadc 3
    fstore 5
    bloadc_t
    bstore 6
    iload_0
    iload_1
    ilt
    branch_f _lab2_otherwise
    iload_2
    iload_3
    igt
    jump _lab3_end
_lab2_otherwise:
    bloadc_f
_lab3_end:
    branch_f _lab0_otherwise
    iloadc_1
    jump _lab1_end
_lab0_otherwise:
    iloadc_0
_lab1_end:
    istore 3
    isrg
    iload_3
    jsre 0
    isrg
    iloadc_1
    jsre 5
    isrg
    iload_0
    iload_2
    ilt
    branch_f _lab4_otherwise
    iloadc_1
    jump _lab5_end
_lab4_otherwise:
    iloadc_0
_lab5_end:
    fload 4
    fload 5
    fadd
    iload_1
    iload_3
    ilt
    branch_f _lab6_otherwise
    iloadc_1
    jump _lab7_end
_lab6_otherwise:
    iloadc_0
_lab7_end:
    jsr 3 _fun0_foo
    istore 3
    isrg
    iload_3
    jsre 0
    isrg
    iloadc_1
    jsre 5
    iload_1
    iload_3
    ilt
    branch_f _lab10_otherwise
    bloadc_t
    jump _lab11_end
_lab10_otherwise:
    iload_0
    iload_2
    igt
_lab11_end:
    branch_f _lab9_end
    iloadc 4
    istore 0
_lab9_end:
    iloadc_1
    ireturn

_fun2_bar:
    esr 3
    iloadc 5
    istore 0
    floadc 6
    fstore 1
    iload_0
    fload_1
    f2i
    iload_0
    imul
    iadd
    istore 2
    isrg
    iload_2
    jsre 0
    isrg
    iloadc_1
    jsre 5
    return

main:
    isrg
    jsr 0 _fun1_fun
    ipop
    isrg
    jsr 0 _fun2_bar
    iloadc_0
    ireturn

.const int 2
.const int 3
.const int 4
.const float 2.000000
.const int 7
.const int 5
.const float 6.000000
.exportfun "main" int main
.importfun "printInt" void int 
.importfun "printFloat" void float 
.importfun "scanInt" int 
.importfun "scanFloat" float 
.importfun "printSpaces" void int 
.importfun "printNewlines" void int 